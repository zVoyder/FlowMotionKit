# FlowMotion Plugin

**FlowMotion** is a plugin for Unreal Engine 5 that provides two modular components for implementing cinematic parkour-inspired movement: **Wall Running** and **Rail Grinding**. The system is fully customizable and serves as a solid base for high-mobility third person games.

---

## Preview

### Wall Run
https://github.com/user-attachments/assets/f7e39457-4b08-485d-a1fe-e110fd2a29b2

### Rail Grind
https://github.com/user-attachments/assets/cf1ab0b6-a13b-48bb-9bc6-dbc77c70b0ae

---

## Components

### 1. `UWallRunner` — Wall Running

Allows the character to run along valid surfaces using physics-based calculations and customizable curve settings.

#### Key Properties

##### Tracing & Attachment
- `TraceChannel`: The collision channel used for wall detection (default: `ECC_Visibility`).
- `CheckRadius`: Radius of the wall detection sphere (default: `75.f`).
- `VelocityToAttach`: Minimum speed required to initiate wall run (default: `450.f`).
- `VelocityToDetach`: Speed threshold below which the character detaches (default: `400.f`).
- `MinWallRunTime`: Minimum duration before detaching is allowed (default: `0.1f`).
- `VerticalInertiaConservation`: Proportion of vertical velocity conserved on attach (0.0 to 1.0).

##### Physics & Movement
- `DefaultGravityScale`: Gravity multiplier during wall run (default: `0.1f`).
- `DefaultMaxSpeedScale`: Max speed multiplier while wall running (default: `1.8f`).
- `DefaultAccelerationScale`: Acceleration multiplier (default: `1.5f`).
- `DefaultStickinessStrength`: Stickiness force toward the wall (default: `4.f`).

##### Launch Settings
- `HorizontalLaunchForce`: Side launch force on detach (default: `800.f`).
- `VerticalLaunchForce`: Vertical force on detach (default: `400.f`).
- `ForwardLaunchScale`: Multiplier for forward direction launch (default: `1.2f`).

##### Curve Settings
- `bUseGravityCurves`: Enables gravity scaling via curve.
- `DefaultGravityMultiplierCurve`: Optional gravity curve (float).
- `bUseSpeedAccelerationCurves`: Enables speed/acceleration modulation via curve.
- `DefaultSpeedAccelerationCurve`: Optional vector curve affecting speed (X) and acceleration (Y).

##### Constraints
- `bAllowsMultipleAttachOnSameWall`: Prevents repeated attachment on the same wall.
- `MaxVerticalAngleDifference`: Max vertical deviation allowed (0–90°).
- `MaxHorizontalAngleDifference`: Max horizontal deviation allowed (0–90°).

##### Advanced
- `AttachInputDelay`: Delay before another attachment is allowed (default: `0.25f`).
- `AttachmentDuration`: Time the character stays attached before evaluating reattach (default: `0.25f`).
- `RotationInterpSpeed`: Interpolation speed for aligning with wall (default: `10.f`).
- `TraceOffset`: Trace origin offset from character.
- `DistanceWeight`: Scoring weight for distance.
- `VerticalAngleWeight`: Weight for vertical wall angle matching.
- `HorizontalAngleWeight`: Weight for horizontal angle matching.

---

### 2. `URailGrinder` — Rail Grinding

Supports dynamic rail grinding on spline-based paths, with customizable speed and launch options.

#### Key Properties

##### Tracing & Attachment
- `TraceChannel`: Collision channel used for spline detection (default: `ECC_Visibility`).
- `CheckRadius`: Radius for trace check (default: `100.f`).
- `DefaultRailOffset`: Offset from rail center (default: `125.f`).
- `MinSpeedToGrind`: Speed threshold for grinding activation (default: `150.f`).
- `DefaultDetachDistance`: Distance before character detaches (default: `200.f`).

##### Movement
- `DefaultRailSpeed`: Base movement speed along rail (default: `500.f`).

##### Launch Settings
- `HorizontalLaunchForce`: Horizontal force when leaving rail (default: `800.f`).
- `VerticalLaunchForce`: Vertical launch on detach (default: `1200.f`).
- `ForwardLaunchScale`: Directional scale for launch (default: `1.0f`).

##### Advanced
- `TraceOffset`: Offset for trace location (default: `FVector(0.f, 0.f, -90.f)`).
- `AttachInputDelay`: Delay to prevent repeated attachments (default: `0.25f`).
- `DistanceWeight`: Weight for evaluating closest rail.
- `AngleWeight`: Weight for angle alignment score.

---

### 3. `ARailSpline` — Spline-Based Rail Generator

An actor that procedurally generates a spline-based mesh for rail grinding.

#### Key Properties

##### Mesh
- `MeshToUse`: The static mesh used for the spline.
- `MaterialOverride`: Optional material to override the mesh default.
- `MeshForwardAxis`: Axis direction of the mesh (default: `X`).

##### Collision
- `CollisionProfileName`: Collision profile applied to spline meshes (default: `BlockAll`).
- `CollisionEnabled`: Type of collision enabled (default: `QueryAndPhysics`).

##### Internal Components
- `SplineComponent`: The spline path.
- `SplineMeshComponents`: Mesh segments generated per spline section.

##### Editor Features
- Overrides `OnConstruction`, `PostEditChangeProperty`, `PostEditMove`, `PostEditUndo`, and `PostDuplicate` to support live updates and editing in editor.

##### Core Methods
- `GenerateMeshes()`: Generates spline mesh components along the spline.
- `ClearMeshes()`: Clears existing spline mesh components.

---

### 4. `URunnableWall` — Wall Definition Tag

A lightweight component used to mark world geometry as valid runnable walls and override default wall running parameters.

#### Key Properties

- `bIsEnabled`: Toggles wall run functionality on this surface.
- `WallGravityScaleOverride`: Optional override for wall gravity scale.
- `WallGravityCurveOverride`: Optional gravity curve override. Takes precedence over `WallGravityScaleOverride`.
- `WallSpeedAccelerationCurveOverride`: Optional speed/acceleration vector curve override.
- `StickinessStrengthOverride`: Custom stickiness force override (default: `4.f`).

#### Internal Flags (Set Internally)

- `bOverrideWallGravityScale`: Whether the wall gravity scale override is active.
- `bOverrideWallGravityCurve`: Whether the wall gravity curve override is active.
- `bOverrideWallSpeedAccelerationCurve`: Whether the speed/acceleration curve override is active.
- `bOverrideStickinessStrength`: Whether the stickiness override is active.

#### Methods

- `HasWallGravityScaleOverride()`: Checks if gravity scale override is active.
- `HasWallGravityCurveOverride()`: Checks if gravity curve override is active.
- `HasWallSpeedAccelerationCurveOverride()`: Checks if speed/acceleration curve override is active.
- `HasStickinessStrengthOverride()`: Checks if stickiness override is active.

---

### 5. `UGrindableRail` — Rail Definition Tag

A lightweight component used to mark spline-based rails as grindable and override default grind behavior.

#### Key Properties

- `bIsEnabled`: Toggles rail grind functionality on this component.
- `RailOffsetOverride`: Optional override for character offset from spline (default: `125.f`).
- `RailSpeedOverride`: Optional override for grind speed (default: `500.f`).
- `DetachDistanceOverride`: Optional override that defines how close to the end of the spline the character must be before detaching (default: `200.f`).

#### Internal Flags (Set Internally)

- `bOverrideRailOffset`: Whether the offset override is active.
- `bOverrideSpeed`: Whether the speed override is active.
- `bOverrideDetachDistance`: Whether the detach distance override is active.
- `SplineComponent`: Reference to the associated spline.

#### Methods

- `GetClosestDistanceOnSpline(WorldLocation)`: Returns closest spline distance from world position.
- `GetSplineComponent()`: Returns the spline component reference.
- `HasRailOffsetOverride()`: Checks if offset override is active.
- `HasRailSpeedOverride()`: Checks if speed override is active.
- `HasDetachDistanceOverride()`: Checks if detach distance override is active.

---

## Installation

1. Clone or copy the `FlowMotion` plugin into your project’s `Plugins/` folder.
2. Regenerate project files and recompile.
3. Enable the plugin in your project settings.

---

## Notes

- Built with `CharacterMovementComponent` integration for full physics compatibility.
- Extensively customizable via Blueprints.
- Modular design allows extension via inheritance or messaging.

---
