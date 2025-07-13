# PAVJ-UserInterface

Este repositorio contiene la implementación completa de un **árbol de habilidades** en Unreal Engine 5.5.3 usando C++ para la lógica y UMG para la interfaz.

---

## 1. Descripción General

El objetivo de este proyecto es ofrecer un menú accesible durante el juego que permite al jugador desbloquear mejoras organizadas en un árbol de tres ramas (Velocidad, Salud, Daño). Cada mejora tiene un coste en puntos y dependencias claras:

- **Ramas**:
  - `Speed_`: mejora de velocidad
  - `Health_`: mejora de salud
  - `Damage_`: mejora de daño

- **Reglas**:
  1. Cada rama tiene 2–3 nodos con prerequisitos secuenciales.
  2. Solo los nodos sin prerequisitos (las tres raíces) están desbloqueados al inicio.
  3. Para desbloquear un nodo, se requieren los puntos suficientes y haber comprado el prerequisito.
  4. Feedback instantáneo si no hay puntos o el nodo está bloqueado/comprado.
  5. Confirmación mediante modal antes de gastar puntos.

---

## 2. Estructura de Clases

### 2.1 C++ Core

- **`USkillTreeComponent`** (`UActorComponent`)
  - Propiedades `EditAnywhere`:
    - `int32 RemainingPoints` – puntos actuales del jugador.
    - `TArray<FSkillNode> SkillNodes` – lista de nodos con:
      - `FName ID`
      - `FText Name`
      - `int32 Cost`
      - `TArray<FName> Prerequisites`
      - `int32 BranchIndex`
      - `ESkillState State`
  - Métodos:
    - `BeginPlay()` – inicializa el estado `Unlocked` de nodos sin prerequisitos.
    - `bool CanPurchase(FName ID)` – comprueba puntos y estado.
    - `bool Purchase(FName ID)` – descuenta puntos, cambia estado y desbloquea dependientes.

- **`AUserInterfacePlayerController`** (`APlayerController`)
  - Propiedad `SkillTreeWidgetClass` (`TSubclassOf<USkillTreeWidget>`).
  - Lógica de input en `SetupInputComponent()` (`ToggleSkillTree`).
  - `ToggleSkillTree()`:
    - Pausa/despausa el juego (`SetPause`).
    - Cambia `InputMode` entre `GameOnly` y `UIOnly`.

- **Widgets en C++**:
  - `USkillTreeWidget` (`UUserWidget`): genera dinámicamente los `USkillNodeButton` y el texto de puntos (`PointsText`).
  - `USkillNodeButton` (`UUserWidget`): muestra nombre, coste y colorea según rama, sólo habilitado si `Unlocked`.
  - `UConfirmPurchaseWidget` (`UUserWidget`): modal de confirmación con `ConfirmText`, `ConfirmButton` y `CancelButton`, delega `OnConfirmed`.

### 2.2 Blueprints (UMG)

- **`WBP_SkillTree`** (parent: `SkillTreeWidget`)
  - Canvas con:
    - 3 `VerticalBox` (`BranchBoxes[0..2]`)
	- 1 `HorizontalBox` (`RemainingPoints`, `FeedbackText`).

- **`WBP_SkillNodeButton`** (parent: `SkillNodeButton`)
  - Jerarquía: `RootButton` con una `VerticalBox` la cual contiene:
    - `NameText`: con el nombre de la skill.
	- `CostText`: con el coste de la skill.

- **`WBP_ConfirmPurchase`** (parent: `ConfirmPurchaseWidget`)
  - Modal con `ConfirmText`, `ConfirmButton` y `CancelButton`.

---

## 3. Ejemplo de SkillNodes

Aquí un conjunto de 11 nodos para probar:

| Branch | ID         | Name          | Cost | Prerequisites     | BranchIndex |
|:------:|:-----------|:--------------|:----:|:------------------|:-----------:|
| Vel    | `Speed_1`  | Velocidad I   |  1   | []                | 0           |
| Vel    | `Speed_2`  | Velocidad II  |  2   | [`Speed_1`]       | 0           |
| Vel    | `Speed_3`  | Velocidad III |  3   | [`Speed_2`]       | 0           |
| Vel    | `Speed_4`  | Velocidad IV  |  5   | [`Speed_3`]       | 0           |
| Sal    | `Health_1` | Salud I       |  1   | []                | 1           |
| Sal    | `Health_2` | Salud II      |  2   | [`Health_1`]      | 1           |
| Sal    | `Health_3` | Salud III     |  3   | [`Health_2`]      | 1           |
| Sal    | `Health_4` | Salud IV      |  5   | [`Health_3`]      | 1           |
| Daño   | `Damage_1` | Daño I        |  3   | []                | 2           |
| Daño   | `Damage_2` | Daño II       |  5   | [`Damage_1`]      | 2           |
| Daño   | `Damage_2` | Daño III      | 10   | [`Damage_2`]      | 2           |

Ajusta `RemainingPoints = X` para disponer de más o menos puntos para desbloquear habilidades.

También puedes añadir o quitar nodos en el apartado `Skill Nodes`.

---

## 4. Personalizaciones

- **Colores de rama**: azul/verde/rojo según prefijo (`Speed_`, `Health_`, `Damage_`).
- **Notificaciones**:
  - Mensaje en pantalla si faltan puntos.
  - Si un nodo está bloqueado la propia habilidad saldrá deshabilitada.
- **Pausa e Input**:
  - Al abrir el menú, `SetPause(true)` y `InputModeUIOnly` con foco en el widget.
  - Al cerrar, `SetPause(false)` y `InputModeGameOnly`.

---

## 5. Buenas Prácticas

- **Separación** de lógica (C++) y diseño (UMG). La idea era tener la lógica del SkillTree en un componente y desacoplar la lógica lo máximo posible de la interfaz.
- Exponer en `EditAnywhere` para iterar rápido desde el Editor.

---

> **¡Listo!**  Clona este repositorio, compila y lanza. Pulsa **Tab** para ver y probar el árbol de habilidades.  
> Cualquier duda o sugerencia, revisa las secciones anteriores o contácta conmigo.
