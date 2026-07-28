# 🌲 DESOLATE — Game Design Document & Documentație Tehnică

> **DESOLATE** este un joc de strategie, management de resurse și supraviețuire tactică post-apocaliptică. Proiectul reinterpretează genul prin eliminarea sistemelor clasice de orientare automată și introducerea navigării manuale pe hartă desenabilă, coordonării prin frecvențe radio și detecției inamice bazate pe semnale sonore și vizuale.

---

## 📋 Cuprins
- [1. Descrierea Problemei și a Soluției](#1-descrierea-problemei-și-a-soluției)
- [2. Analiza Pieței și Publicul Țintă](#2-analiza-pieței-și-publicul-țintă)
- [3. Arhitectura Aplicației și Design Pattern-uri](#3-arhitectura-aplicației-și-design-pattern-uri)
- [4. Mecanici de Joc și Sisteme Principale](#4-mecanici-de-joc-și-sisteme-principale)
- [5. Stabilitate, Performanță și Securitate](#5-stabilitate-performanță-și-securitate)
- [6. Strategia de Testare și Bug Tracking](#6-strategia-de-testare-și-bug-tracking)
- [7. Ghid de Instalare și Configurare](#7-ghid-de-instalare-și-configurare)
- [8. Justificări Obligatorii (Cerințe InfoEducație)](#8-justificări-obligatorii-cerințe-infoeducație)
- [9. Roadmap și Versionare (Git)](#9-roadmap-și-versionare-git)
- [10. Resurse Externe și Atribuiri](#10-resurse-externe-și-atribuiri)
- [11. Evaluare Arhitecturală și Puncte Tari](#11-evaluare-arhitecturală-și-puncte-tari)

---

## 1. Descrierea Problemei și a Soluției

### 1.1 Problema Identificată
Jocurile moderne de strategie și supraviețuire oferă jucătorilor informații perfecte ("God view"): hărți centrate automat, minimap-uri cu GPS, markere explicite de misiune și navigare de tip point-and-click fără risc de rătăcire. Această abordare reduce senzația autentică de izolare, tensiunea tactică și necesitatea de planificare strategică riguroasă.

### 1.2 Soluția Propusă: DESOLATE
**DESOLATE** plasează jucătorul în rolul unui dispecer / comandant izolat într-un adăpost post-apocaliptic. Locația airdropurilor și a outposturilor se poate afla prin **radio**, iar traseele trupelor trebuie **desenate manual** de jucător pe o hartă schematică cu axe X/Y. Jocul combină tensiunea psihologică cu planificarea resurselor în timp real.

---

## 2. Analiza Pieței și Publicul Țintă

### 2.1 Publicul Țintă
* Pasionați de jocuri de strategie tactică (RTS) și supraviețuire (*Frostpunk*, *RimWorld*, *Radio Commander*).
* Jucători care apreciază mecanici de gestiune a resurselor și atmosfere imersive de mister/horror tactic.

### 2.2 Analiză Comparativă (Competiție)

| Caracteristică | Jocuri RTS Clasice | Frostpunk / RimWorld | **DESOLATE** |
| :--- | :--- | :--- | :--- |
| **Control Trupe** | Selectare directă + click | Management indirect / Sarcini | **Trasare manuală de rute pe hartă (Traced Paths)** |
| **Vizibilitate** | Fog of War standard | Hartă complet vizibilă / Clădiri | **Invert Scanning & Avertismente Audio** |
| **Navigație** | Căutare de drum automată | Automată | **Path Following** |
| **Tensiune** | Bazată pe număr de unități | Bazată pe indicatori / Frig | **Tensiune psihologică (audio & inamici cu AI complex)** |

---

## 3. Arhitectura Aplicației și Design Pattern-uri

Aplicația este proiectată într-un mod strict modular, combinând un model de tip ECS Lite (inspirat din arhitectura GameObject–Component / MonoBehaviour din Unity) cu servicii de-cuplate centralizate.

```
                    +--------------------------------+
                    |    Context (Service Locator)   |
                    +---------------+----------------+
                                    |
            +-----------------------+-----------------------+
            |                       |                       |
    +-------v-------+       +-------v-------+       +-------v-------+
    |   MapSystem   |       |  RadioSystem  |       | SquadManager  |
    +-------+-------+       +-------+-------+       +-------+-------+
            |                       |                       |
+-----------v-----------------------v-----------------------v-----------+
|                          ENTITY CONTAINER                             |
|  +-------------------+  +-------------------+  +-------------------+  |
|  | AudioComponent    |  |  PathTracerComp.  |  |  HealthComponent  |  |
|  |  - update()       |  |   - update()      |  |   - update()      |  |
|  +-------------------+  +-------------------+  +-------------------+  |
+-----------------------------------------------------------------------+
```

### 3.1 Arhitectura ECS Lite & Extensibilitate Modulară
Proiectul folosește un model de tip **ECS Lite** (inspirat din arhitectura bazată pe componente din Unity). Entitățile (Entity) acționează ca containere generice de date, iar logica și comportamentele sunt încapsulate în componente modulare ce-și gestionează propriul ciclu de viață (update(), draw()).

Prin această decuplare totală, adăugarea de caracteristici noi se rezumă la alipirea de componente pe entitate, fără rescrierea claselor existente sau moșteniri rigide:
## ECS Lite Component System

Fiecare funcționalitate e un modul izolat (`HealthComponent`, `ScanComponent`, `FactionComponent`). Se atașează dinamic:

```cpp
entity->addComponent<HealthComponent>(100.f, 100.f);
entity->addComponent<ScanComponent>();
```

`Entity` deține `vector<unique_ptr<Component>>`, iar `getComponent<T>()` face căutare cu `dynamic_cast`. Fiecare `Component` are `update()` și `draw()` virtuale.

## Strategy Pattern (AI Behavior)

`StrategyDriver` (o componentă) deține un `Strategy*` curent și comută între strategii via `setStrategy()`. Fiecare monstru are propriul driver cu enum de stări:

- `WandererStrategyDriver` — `PATH_FOLLOW`, `CHASE`
- `TerritorialStrategyDriver` — `PATROL`, `CHASE`
- `LurkerStrategyDriver` — `IDLE`, `MOVE_TO_RESOURCE`, `PATROL`, `CHASE`
- `HunterStrategyDriver` — `IDLE`, `HUNT`, `RETURN_TO_LAIR`

```cpp
class WandererStrategyDriver : public StrategyDriver {
    enum State { PATH_FOLLOW, CHASE };
    void update(Context& context) override {
        switch (state) {
        case PATH_FOLLOW:
            if (chaseStrategy.findNearestEnemy(context)) {
                setStrategy(&chaseStrategy);
                state = CHASE;
            }
            break;
        case CHASE:
            if (!context.isEntityValid(chaseStrategy.chasedEntity)) {
                setStrategy(&pathFollowerStrategy);
                state = PATH_FOLLOW;
            }
            break;
        }
        currentStrategy->update(context);
    }
};
```

## Scene Stack

Navigarea între ecrane se face printr-o stivă de `Scene`. Fiecare `Scene` conține un `Context` și un flag `pendingPop`. Bucla principală rulează scena din vârf:

```cpp
sceneStack.push(createMenuScene(...));
while (window.isOpen()) {
    Scene* top = sceneStack.topScene();
    Context& ctx = top->context;
    ctx.update();
    ctx.entityUpdate();
    if (ctx.gameOver) { sceneStack.pop(); sceneStack.push(createGameOverScene(...)); }
    ctx.entityDraw(window, {});
    window.display();
    while (top->pendingPop) { sceneStack.pop(); ... }
}
```

Scene: `createMenuScene`, `createPlayScene`, `createSettingsScene`, `createGameOverScene`, `createVictoryScene`, `createDebugScene`.

## Factory Pattern

Entitățile și scenele sunt create prin funcții factory:

```cpp
namespace Desolate::Factory {
    Entity* createSquadEntity(...);
    Entity* createWandererEntity(...);
    Entity* createHunterEntity(...);
    Entity* createOutpostEntity(...);
    Entity* createMapEntity(...);
    // ...
}
namespace Desolate::SceneFactory {
    Scene* createPlayScene(window, input, fonts, settings, seed);
    Scene* createMenuScene(window, input, fonts, stack, settings);
    // ...
}
```

## Quest System

Quest-urile sunt o listă înlănțuită de `QuestNode`-uri (cu `prev`/`next`). `Questline` le procesează secvențial, apelând `checkRequirement()` și `applyReward()` pe nodul curent.

Noduri concrete: `KillCountQuest`, `ResourceThresholdQuest`, `OutpostCaptureQuest`, `SquadCountQuest`, `SpecificKillQuest`, `SurvivalTimeQuest`, `ObjectivePickupQuest`, `OutpostCaptureAllQuest`, `InstantQuest`.

## Settings State

Toți parametrii configurabili (video, volum, debug flags) sunt într-un `struct` cu `load()`/`save()` și flag-uri `pending*` pentru resetarea ferestrei.

---

## 4. Mecanici de Joc și Sisteme Principale

### 4.1 Managementul Adăpostului și Resurselor
- **Resurse:** Food, Metal, People (coloniști). Resursele încep cu valori implicite (50 food, 5 people, 0 metal).
- **Ciclul de tick:** La fiecare tick, supraviețuitorii consumă food. Dacă un supraviețuitor e alocat la muncă (working), consumul e accelerat. Supraviețuitorii care muncesc produc metal conform unui rate. Resursele pot fi colectate și din Resource Cache-uri de pe hartă.
- **Upgrade-uri:** Metalul e cheltuit pentru upgrade-uri permanente: view buff (rază de detectare), max HP, supply max, damage, food efficiency, metal production.
- **Sistemul de supply al trupelor:** Fiecare squad are un supply care se drenează constant. Când ajunge la 0, squad-ul pierde HP progresiv (procent din max HP). Supply-ul se reîncarcă la outpost-uri sau când culeg orice resursă.
- **Outpost-urile:** Avanposturi neutre capturabile — o trupă a jucătorului intră în raza outpost-ului și-l capturează (schimbă facțiunea). Odată capturat, oferă în rază: vindecare (HealComponent), reîncărcare supply (SupplyReplenishComponent), reîncărcare shockwave (ShockwaveRechargeComponent), și protecție (ProtectComponent).
- **Sistemul de protecție (ProtectionSystemComponent):** Un sistem global care verifică toate entitățile cu ProtectComponent. Entitățile care au `protectsOthers = true` devin protectoare. Orice altă entitate cu `isProtected = true` e considerată protejată doar dacă se află în raza de protecție a unui protector. De asemenea monștrii vor evita zonele protejate.

### 4.2 Harta Desenabilă și Navigarea
- **Harta desenabilă:** O entitate separată cu MapDrawingComponent. Jucătorul poate desena/șterge pe o textură suprapusă hărții — marchează poziții, trasează drumuri, notează inamici.
- **Trasarea de drumuri pentru trupe:** Jucătorul selectează o trupă și trasează o linie pe hartă. Linia e eșantionată în noduri (TracedPathNode) care formează un TracedPath. Trupa urmărește nodurile secvențial printr-un PathFollowerComponent.
- **Sistemul de coordonate:** Harta are limite fixed (MAP_WIDTH x MAP_HEIGHT). WorldComponent face conversia între coordonatele lumii și cele ale viewport-ului hărții (proiecție).
- **Fog of War:** Entitățile au VisibilityComponent cu o rază și un timp de appear. FogofWarComponent gestionează vizibilitatea — zonele nevăzute sunt ascunse.

### 4.3 Sistemul de Quest-uri (Obiective)
- **Structură:** Quest-urile sunt organizate într-un questline liniar. Fiecare quest (QuestNode) are un nume, un obiectiv, și verifică o condiție (`checkRequirement`). La îndeplinire, aplică o recompensă (`applyReward`) și trece la următorul nod.
- **Progresia:** QuestSystemComponent procesează toate questline-urile activ. Când nodul curent e satisfăcut, se avansează la următorul. Când ultimul nod e completat, questline-ul e marcat `completed` și se declanșează victoria (`context.victory = true`).
- **Tipuri de quest-uri:** Capturează outpost-uri, ucide un număr de inamici (general sau specific — Hunters), acumulează resurse (food, metal), menține un număr de squads, capturează toate outpost-urile, supraviețuiește un timp, ridică obiective, escape (instant win).
- **HUD:** QuestSystemComponent generează o listă de `QuestLineEntry` cu status (Completed/Current/Future) afișată în UI.

### 4.4 Bestiar & I.A. Inamică

- **Wanderer (comun):** Patrulează pe un drum ciclic între 3-4 puncte random generate la spawn. Când detectează o entitate inamică în raza de aggro, comută în chase — urmărește ținta. Are un timer de de-aggro care scade la primirea de damage. După ce timer-ul expiră și ținta e pierdută, reia patrularea. Se vindecă pasiv în regim de patrulare.
- **Territorial:** Patrulează aleatoriu într-un cerc în jurul punctului de spawn. Chase asemănător cu Wanderer-ul, dar nu se îndepărtează de origină. Atacă trupele care intră pe teritoriul său.
- **Lurker:** Pornește în stare IDLE. Detectează Resource Cache-uri și Airdrop-uri și se deplasează la ele (MoveToEntityStrategy). Odată ajuns, resursa e distrusă după un timp. Dacă o trupă se apropie, comută în chase. Are și un mod de patrulare odată ajuns la destinație.
- **Hunter:** Aproape invizibil. Pornește din lair (HunterLairComponent), vânează trupe global pe hartă (GlobalScanComponent + HuntStrategy). Dacă pierde ținta, se întoarce la lair. Poate fi omorât numai prin shockwaveuri atunci când atacă o unitate.
- **Sistem de detectare:** Bazat pe ScanComponent (rază, listează entități) + FactionComponent (facțiuni). La detectarea unui inamic nou, se declanșează un voice-line "Spotted" prin AudioComponent.
- **Sistem de auz (HearComponent):** O rază pasivă mai mare decât scanarea vizuală. Detectează entități inamice apropiate fără contact vizual direct. Folosit de trupe pentru a atrage atenția.
- **Atac:** Inamicii folosesc TimedAttackComponent (atacuri la interval regulat), iar trupele StillAttackComponent (atacuri la interval regulat doar dacă unitatea stă pe loc).
- **Radio Events:** Evenimente periodice pe frecvențe radio — OutpostRadioEvent (semnalizează outpost-uri necapturate), AirdropRadioEvent (aeriale cu resurse). Jucătorul trebuie să acorde frecvența corectă pe UI-ul radio pentru a le declanșa
---

## 5. Stabilitate, Performanță și Securitate

### Stabilitate

- **Validare entități:** `Context::isEntityValid()` verifică existența entității într-un `std::set` înainte de orice operație, prevenind accesul la pointeri șterși.
- **Adăugare/ștergere amânată:** Entitățile sunt colectate în `pendingAdditions` și procesate batch în `flushPendingAdditions()`. Ștergerea se face prin flag `markedForDeletion`, iar entitățile marcate sunt colectate și șterse după bucla de update. Asta previne invalidarea iteratoarelor.
- **Cast sigur:** `Entity::getComponent<T>()` folosește `dynamic_cast` pe fiecare componentă, returnând `nullptr` la eșec — toți apelanții verifică null înainte de utilizare.
- **Fallback la poziție:** `getLogicPosition()` încearcă `WorldPositionComponent`, iar dacă lipsește, folosește `Entity::position`.
- **Cicluri de viață:** `SceneStack` și `Context` fac cleanup automat în destructori. Componentele sunt deținute prin `unique_ptr` în `Entity`.

### Performanță

- **Ordine de update:** Entitățile au `updatePriority` — la inserare, Context le plasează sortat în vector. Sistemele critice (death, protection, fog) au prioritate mică, UI-ul are prioritate mare.
- **Skipping:** Entitățile cu `disabled = true` sunt complet ignorate în `entityUpdate()`. Cele cu `hidden = true` sunt ignorate la draw.
- **Generare procedurală pe chunk-uri:** Harta e împărțită în chunk-uri (grid de CHUNK_COLS x CHUNK_ROWS). Generarea folosește un RNG seeded deterministic per chunk (`seed ^ (gridX * 31 + gridY * 17)`), permițând populație consistentă fără salvare.
- **DeltaTime:** Toate mișcările și timerele folosesc `context.deltaTime`, asigurând comportament consistent indiferent de framerate.
- **Scene Stack:** Doar scena din vârf rulează și se desenează. Scenele anterioare sunt suspendate automat.
- **Fog of War:** Entitățile nedetectate nu sunt desenate, reducând draw calls.

### Securitate

- **Value clamping:** Resursele și valorile sunt limitate prin `std::clamp` (ex: `HealthComponent::changeHealth`, `SupplyComponent::changeSupply`).
- **Validare date externe:** La parsarea input-ului de seed din UI, `std::stoi` e înconjurat de try/catch.
- **Persistență settings:** Fișierul de settings e scris/citit în text simplu, fără execuție de cod.
- **Gestiune memorie:** Entitățile sunt create cu `new` și șterse în `Context::~Context()` sau prin mecanismul `markedForDeletion`. Componentele sunt deținute prin `unique_ptr`. `SceneStack` face delete pe toate scenele la destrucție.

---

## 6. Build și Distribuire

### Cerințe sistem
- **Compiler:** MSVC (Visual Studio 2022 sau Build Tools 2022)
- **CMake** 3.28+
- **Git**
- **OS:** Windows 10/11

### Pași de build

```cmd
git clone <url-repo>
cd Desolate
cmake -B build
cmake --build build --config Release
```

Executabilul se generează în `build/bin/Release/main.exe`. SFML 3.1.0 se descarcă și linkează static automat prin `FetchContent` — nu e nevoie de instalare manuală sau DLL-uri.

### Creare pachet portabil

Rulezi scriptul inclus:

```cmd
package.bat
```

Acesta build-uiește Release și creează un folder `Desolate-Package/` care conține doar `main.exe` + `resources/`. Totul e gata de distribuit — fără dependențe externe.

**Structura finală a pachetului:**

```
Desolate-Package/
├── main.exe
└── resources/
    ├── textures/    (.png)
    ├── fonts/       (.ttf)
    └── audio/       (.ogg, .mp3, .wav)
```

---

## 7. Justificări Obligatorii

### 7.1 Justificarea Tehnologiilor Alese

S-a ales o arhitectură **Entity-Component proprietară** peste **SFML 3.1**, renunțând la Unity/Unreal din următoarele motive:

- **Control total asupra memoriei și ciclului de viață:** Entitățile sînt create manual (`new`/`delete`), componentele sînt deținute prin `unique_ptr`, iar actualizarea se face iterativ printr-un `vector<Component>`.
- **Fără dependențe externe la runtime:** SFML e linkat static (`sfml-graphics-s.lib`), iar toate resursele (texturi, fonturi, sunete) sînt încărcate din fișiere locale. Aplicația finală e un singur `.exe` de ~6 MB + folderul `resources/` — zero DLL-uri, zero runtime-uri.
- **Build time redus:** Proiectul are ~40 de fișiere sursă C++ și se compilează în sub 10 de secunde cu MSVC. Schimbările incrementale durează 1–2 secunde, permițând iterații rapide fără link time îndelungat.
- **Versatilitatea SFML:** Oferă exact ce e necesar — randare 2D (`sf::RenderWindow`, `sf::Texture`, `sf::Sprite`, `sf::Shader`), sunet 3D-like (`sf::Sound`, `sf::Music`), input (`sf::Event`, `sf::Keyboard`/`sf::Mouse`) și matematică vectorială (`sf::Vector2`, `sf::FloatRect`) — fără modul de particule, UI toolkit sau sistem de animații impuse.

### 7.2 Opinia Autorilor despre Proiect și Utilitatea sa
Consider că „DESOLATE” aduce o gură de aer proaspăt în categoria aplicațiilor de divertisment și strategie, transformând o interfață aparent simplă și minimalistă într-o experiență tactică profundă. Prin eliminarea automatismelor moderne și reintroducerea nevoii de orientare manuală (trasare pe hartă, ascultarea alertelor radio), proiectul dezvoltă atenția distributivă, gândirea strategică sub presiune și capacitatea de optimizare a resurselor. Considerăm că aplicația își atinge cu succes publicul țintă, oferind o provocare intelectuală și o atmosferă imersivă unică.

---

## 8. Roadmap

## 8.1 Planuri de viitor pentru direcția artistică
-Urmărirea unei atmosfere care înfricoșează prin voicelineuri în plus, sunete pentru monștrii, dar și shadere și alte îmbunătățiri pentru a face UI-ul sa pară a fi suprafața unei mese într-o lume 3D.
-Mai mult polish pentru sistemele deja existente cum ar fi butoanele de upgrade, radioul și questurile prin texturi noi, sound effects și animații.
-Questurile să fie introduse prin tooluri de comunicare deja existente (radio) sau care pot fi obținute, fiecare quest având voicelines custom.

## 8.2 Planuri de viitor pentru gameplay
-Adăugarea a mai multor inamici precum unii care lucrează în grup sau a unei noi facțiuni care este ostilă tuturor.
-Mărirea ariei de joc prin crearea unei hărți mai mari care poate fi parcursă ca în restul jocurilor de tip RTS.
-Dezvoltarea sistemului de procedural generation prin chunkuri noi și garanții care au drept scop sporirea experienței jucătorului (dispersarea relativ egală a inamicilor și resurselor).

---

## 9. Resurse Externe și Atribuiri

* **Biblioteci C++ / Frameworks:** SFML 3.2.
* **Efecte Audio:** Sunete radio de tip static, focuri de armă și zgomote de fundal preluate de pe [Freesound.org](https://freesound.org/) (Licență CC0 / Creative Commons Public Domain).
* **Texturile:** Au fost făcute cu AI.
* **Voicelines:** Prieteni care și-au dat consimțământul ca vocea lor să apară în joc.

---
---
*Proiect dezvoltat pentru Concursul Național de Informatică și Tehnologie **InfoEducație** — Secțiunea Software Utilitar.*