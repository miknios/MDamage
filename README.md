# MDamage

A generic library for dealing damage to Actors in Unreal Engine in a designer-configurable way.

## Overview

MDamage provides a flexible, component-based system for implementing damage mechanics in your Unreal Engine projects. It allows you to easily make actors damageable, configure damage types, apply effects on damage/death, and integrate with team-based systems for affiliation filtering.

The plugin is designed to be extensible and integrates seamlessly with other M-series plugins (MUtility, MTeam, MSimpleGAS) for a complete gameplay framework.

## Features

### Core Damage System
- **UMDamageableComponent**: Attach this component to any actor to make it damageable with health management
- **Health Attributes**: Automatic health and max health attributes using MSimpleGAS
- **Damage Configuration**: Configurable damage amounts, team affiliation filters, and damage effects
- **Invulnerability System**: Multi-source invulnerability with easy add/remove controls

### Effects System
- **On Damage Effects**: Apply effects to the damaged actor (e.g., blink, sound, particles) with execution filters (always, non-death only, death only)
- **Deal Damage Effects**: Apply effects when dealing damage (e.g., knockback, elemental damage)

### Animation Integration
- **Anim Collision**: Detect overlaps during animations and trigger damage or other effects
- **Combo Attacks**: Manage combo sequences with input gathering, unskippable segments, and progression tracking

### Events & Delegates
- OnDamaged, OnHealed, OnDeath, OnHealthReset, OnInvulnerabilityChanged events
- Blueprint-friendly delegates for easy integration

### Utilities
- Blueprint library functions for dealing damage and getting components
- Debug visualization for damage colliders
- Reset functionality for gameplay state management

## Dependencies

This plugin requires the following M-series plugins:
- **MUtility**: Core utilities and interfaces
- **MTeam**: Team affiliation and filtering system
- **MSimpleGAS**: Simplified Gameplay Ability System for attributes

## Installation

1. Copy the `MDamage` folder to your project's `Plugins` directory
2. Ensure the required dependency plugins are also installed
3. Regenerate project files and build

## Quick Start

### Making an Actor Damageable

1. Add a `UMDamageableComponent` to your actor
2. Configure initial health in the component properties
3. Bind to the `OnDamagedDelegate` and `OnDeathDelegate` for custom logic

### Dealing Damage

```blueprint
// Get the damageable component
UMDamageableComponent* Damageable = UMDamageLibrary::GetDamageableComponent(TargetActor);

// Create damage config
FMDamageConfig DamageConfig(50); // 50 damage

// Create damage instance
FMDamageInstanceData DamageData(DamageConfig, GetActorLocation(), this);

// Deal damage
UMDamageLibrary::DealDamage(DamageData, TargetActor);
```

### Using Damage Config Assets

Create a `UMDamageConfigAsset` in the editor:
- Set damage amount
- Configure team affiliation filter
- Add damage deal effects (e.g., knockback)

### Animation-Based Attacks

1. Add `UAnimNotify_Overlap` to your attack animation
2. Configure collision shape and profile
3. Add `UAnimNotify_OverlapHandler_Damage` as a handler
4. Set the damage config asset

### Combo Attacks

1. Create a `UMComboAttack_ComboRunner` instance
2. Initialize with combo length
3. Bind to progression delegates
4. Use anim notifies to control combo flow:
   - `MComboAttack_InputGather_AnimNotify`: Enable/disable input gathering
   - `MComboAttack_UnskippableAttackSegment_AnimNotify`: Mark unskippable segments

## Advanced Usage

### Custom Damage Effects

Create subclasses of `UMDamageDealEffect_Base` for custom damage-dealing effects:

```cpp
void UMyDamageEffect::ApplyDamageDealEffect_Implementation(UMDamageableComponent* DamagedComponent, const FMDamageInstanceData& DamageData)
{
    // Apply knockback, elemental damage, etc.
}
```

### Custom On-Damage Effects

Subclass `UMDamageableOnDamageEffect_Base` for effects on the damaged actor:

```cpp
void UMyOnDamageEffect::ApplyOnDamageEffect_Implementation(const FMDamageInstanceData& DamageData)
{
    // Play blink animation, spawn particles, etc.
}
```

### Team Affiliation

Use `FMTeamAffiliationFilter` in damage configs to control which teams can damage each other:

```cpp
FMDamageConfig Config(50, FMTeamAffiliationFilter(EAllianceType::Enemy)); // Only damage enemies
```

## API Reference

### Key Classes
- `UMDamageableComponent`: Core damageable component
- `FMDamageConfig`: Damage configuration struct
- `UMDamageConfigAsset`: Data asset for damage configs
- `UMDamageLibrary`: Blueprint utility functions
- `UAnimNotify_Overlap`: Animation collision detection
- `UMComboAttack_ComboRunner`: Combo attack management

### Events
- `FMOnDamagedSignature`: Fired when damage is taken
- `FMOnDeathSignature`: Fired when health reaches zero
- `FMOnHealedSignature`: Fired when healed
- `FMOnHealthResetToMaxSignature`: Fired when health reset
- `FMInvulnerabilityStateChangedSignature`: Fired when invulnerability changes

## License

Copyright (c) Miknios. All rights reserved.
