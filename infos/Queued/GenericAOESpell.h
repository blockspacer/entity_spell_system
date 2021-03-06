#pragma once
#include <System/System.h>
#include "SpellScript.h"
#include "GenericSpellData.h"
#include "SpellHealData.h"
#include "SpellCastEffectData.h"
#include "SpellAOEData.h"
#include "SpellRangeData.h"
#include "Spell.h"
#include "Mathf.h"
#include "Debug.h"
#include "WorldEntity.h"
#include "WorldSpell.h"
#include "Entity.h"

using namespace UnityEngine;
using namespace BS::Player;
using namespace BS::SpellsNS::WorldSpells;
using namespace System;
namespace BS {
	namespace SpellsNS {
		namespace generic {
			class GenericAOESpell : public virtual SpellScript, public virtual Object
			{
				private:
					GenericSpellData* genericSpellAttribute;
				private:
					SpellHealData* spellHealAttribute;
				private:
					SpellCastEffectData* spellCastEffectAttribute;
				private:
					SpellAOEData* aoeAttrib;
				private:
					SpellRangeData* rangeAttrib;
				//Attribute: SerializeField*
				private:
					bool damage;
				//Attribute: SerializeField*
				private:
					bool heal;
				//Attribute: SerializeField*
				private:
					bool hasCooldown;
				//Attribute: SerializeField*
				private:
					bool hasCastTime;
				//Attribute: SerializeField*
				private:
					bool hasSpellCastEffect;
				//Attribute: SerializeField*
				private:
					bool range;
				public:
					GenericAOESpell(Spell* spell);
				public:
				virtual void StartCasting(WorldEntity* caster, float spellScale);
				public:
				virtual void CastingFinished(WorldEntity* caster, float spellScale);
				public:
				virtual void SpellHit(WorldEntity* caster, WorldEntity* target, WorldSpell* worldSpell, Spell* spellGo, float spellScale);
				public:
				virtual void OnPlayerMove(Entity* caster);
				private:
					void HandleSpellEffect(WorldEntity* caster, WorldEntity* target, float spellScale);
				public:
				virtual void COnSpellCastSuccess(WorldEntity* player);
				public:
				virtual void COnSpellCastStarted(WorldEntity* player);
				public:
				virtual void COnSpellCastEnded(WorldEntity* player);
			};
		}
	}
}
