#include "ScriptMgr.h"
#include "Configuration/Config.h"
#include "Player.h"
#include "World.h"
#include "Chat.h"
#include "EventMap.h"

enum spells
{
    SPELL_BANISH = 30231,
    SPELL_SHADOWBOLT_VOLLEY = 39175,
    SPELL_SILENCE = 37160,
    SPELL_PULL = 30010,
    SPELL_INSTA_KILL = 7,
    SPELL_RAIN_OF_FIRE = 43440,
    SPELL_SHADOW_BOLT = 39309,
    SPELL_FIRE_NOVA = 46551,
    SPELL_SLOW = 25603,
    SPELL_TELEPORT_VISUAL = 41232,

    //Events
    EVENT_SPELL_VOLLEY = 1,
    EVENT_SUMMON_CREATURES = 2,
    EVENT_PULL = 3,
    EVENT_INSTA_KILL = 4,
    EVENT_CHECK_HEALTH_PCT = 5,
    EVENT_FIRE_NOVA = 6,

    EVENT_SPELL_RAIN_OF_FIRE = 7,
    EVENT_SHADOW_BOLT = 8, //Target Tank only!

};

#define SUMMON_CREATURE_1 22362
#define SUMMON_CREATURE_2 17400
#define SAY_AGGRO_1 "You think you can scare me! Pathetic!"
#define SAY_AGGRO_2 "Ha, finerly a challange!"
#define SAY_AGGRO_3 "Puny people, prepare to DIE!"
#define SAY_KILL_1 "I feel your soul leave your corpse!"
#define SAY_KILL_2 "Pathetic waste of air!"
#define SAY_DIE "You have proved me wro..."
#define SAY_PULL "Come closer and BURN!"
#define MAX_SUMMONED_CREATURES 32

class TalamortisBoss : public CreatureScript
{
public:
    TalamortisBoss() : CreatureScript("TalamortisBoss") { }

    struct TalamortisBossAI : public ScriptedAI
    {
        TalamortisBossAI(Creature* c) :ScriptedAI(c), summons(me) {}

        SummonList summons;
        EventMap events;
        uint32 SummonedCreatures = 0;

        void Reset()
        {
            summons.DespawnAll();
            events.Reset();
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        void EnterCombat(Unit* /*who*/)
        {
            switch (rand() % 3)
            {
            case 0:
                me->MonsterYell(SAY_AGGRO_1, LANG_UNIVERSAL, NULL);
                break;
            case 1:
                me->MonsterYell(SAY_AGGRO_2, LANG_UNIVERSAL, NULL);
                break;
            case 2:
                me->MonsterYell(SAY_AGGRO_3, LANG_UNIVERSAL, NULL);
                break;
            }
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            
            me->SetRooted(true);
            events.SetPhase(0);
            events.ScheduleEvent(EVENT_SPELL_VOLLEY, 2000);
            events.ScheduleEvent(EVENT_SUMMON_CREATURES, 5000);
            events.ScheduleEvent(EVENT_PULL, 20000);
            events.ScheduleEvent(EVENT_INSTA_KILL, 1000);
            events.ScheduleEvent(EVENT_CHECK_HEALTH_PCT, 1000);
        }

        void KilledUnit(Unit* /*victim*/)
        {

            switch (rand() % 2)
            {
            case 0:
                me->MonsterYell(SAY_KILL_1, LANG_UNIVERSAL, NULL);
                break;
            case 1:
                me->MonsterYell(SAY_KILL_2, LANG_UNIVERSAL, NULL);
                break;
            }
            // For Each player that Dies Give me 10% Life
            uint32 LifeDrain = me->CountPctFromMaxHealth(10);
            me->SetHealth(me->GetHealth() + LifeDrain);
        }

        void JustDied(Unit* /*Killer*/)
        {
            me->MonsterYell(SAY_DIE, LANG_UNIVERSAL, NULL);
            summons.DespawnAll();
        }

        void JustSummoned(Creature* summoned)
        {
            //  Forced Summon Creatures to Attack players
            Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 30, false);
            if (pTarget)
                summoned->AI()->AttackStart(pTarget);

            SummonedCreatures++;
            summons.Summon(summoned);
        }

        void SummonedCreatureDies(Creature* summon, Unit*)
        {
            SummonedCreatures--;

            // For Each Creature Dies Take 1% of life of the boss Only in phase 0
            me->SetHealth(me->GetHealth() - summon->GetMaxHealth());
            summons.Despawn(summon);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            events.Update(diff);

            switch (events.ExecuteEvent())
            {
            case EVENT_SPELL_VOLLEY:
            {
                // Every 5 Seconds Attack Players with Shadow Bolt Volley
                me->CastSpell(me->GetVictim(), SPELL_SHADOWBOLT_VOLLEY, false);
                events.Repeat(5000);
                break;
            }

            case EVENT_SUMMON_CREATURES:
            {
                if (SummonedCreatures < MAX_SUMMONED_CREATURES)
                    me->SummonCreature(RAND(SUMMON_CREATURE_1, SUMMON_CREATURE_2), me->GetRandomNearPosition(10), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000);
                events.Repeat(urand(3000, 5000));
                break;
            }
            case EVENT_PULL:
            {
                me->MonsterSay(SAY_PULL, LANG_UNIVERSAL, NULL);
                //Every 10 Seconds Pull players to the NPC and silence
                ThreatContainer::StorageType const& t_list = me->getThreatManager().getThreatList();
                for (ThreatContainer::StorageType::const_iterator itr = t_list.begin(); itr != t_list.end(); ++itr)
                {
                    if (Unit* target = Unit::GetUnit(*me, (*itr)->getUnitGuid()))
                    {
                        if (!target->IsAlive())
                            continue;

                        me->CastSpell(target, SPELL_TELEPORT_VISUAL, false);
                        target->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), target->GetOrientation());
                        target->AddAura(SPELL_SILENCE, target);
                        target->AddAura(SPELL_SLOW, target);
                        events.ScheduleEvent(EVENT_FIRE_NOVA, 2000);
                    }
                }
                
                events.Repeat(20000);
                break;
            }
            case EVENT_INSTA_KILL: // Check Threat Table for Furthest away
            {
                Unit* pTarget = SelectTarget(SELECT_TARGET_FARTHEST, 0, 30, false);
                if (pTarget->GetDistance2d(me) >= 20)
                    //pTarget->CastSpell(pTarget, SPELL_INSTA_KILL, false);
                    me->Kill(pTarget, true);

                events.Repeat(1000);
                break;
            }
            case EVENT_CHECK_HEALTH_PCT:
            {
                if (events.GetPhaseMask() == 0)
                {
                    if (me->GetHealthPct() <= 50)
                    {
                        events.SetPhase(1);
                        me->SetRooted(false);
                        me->RemoveAurasDueToSpell(SPELL_BANISH);
                        events.ScheduleEvent(EVENT_SHADOW_BOLT, 2000);
                        events.ScheduleEvent(EVENT_SPELL_RAIN_OF_FIRE, 8000);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }
                }
                events.Repeat(1000);
                break;
            }
            case EVENT_SHADOW_BOLT:
            {
                me->CastSpell(me->GetVictim(), SPELL_SHADOW_BOLT, false);
                events.Repeat(urand(2000, 4000));
                break;
            }
            case EVENT_SPELL_RAIN_OF_FIRE:
            {
                Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 30, false);
                if (pTarget)
                    me->CastSpell(pTarget, SPELL_RAIN_OF_FIRE, false);

                events.Repeat(urand(8000, 15000));
                break;
            }
            case EVENT_FIRE_NOVA:
            {
                me->CastSpell(me, SPELL_FIRE_NOVA, false);
                break;
            }
            }

            if (events.IsInPhase(1))
                DoMeleeAttackIfReady();

        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new TalamortisBossAI(creature);
    }
};

void Addmod_custom_bossScripts()
{
    new TalamortisBoss();
}