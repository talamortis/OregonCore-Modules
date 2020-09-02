DROP TABLE IF EXISTS `npc_buff_spells`;

CREATE TABLE `npc_buff_spells` (
  `spell_id` int(5) NOT NULL default '0' COMMENT 'Spell entry number',
  `cat_number` int(5) NOT NULL default '0' COMMENT '0-small buff 1-great buff 2-gm buff 3-player tools',
  `cost` int(10) NOT NULL default '0' COMMENT 'cost amount in copper',
  `name` text COMMENT 'Name of spell',
  PRIMARY KEY  (`spell_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

/*Data for the table `npc_buff_spells` */

INSERT INTO `npc_buff_spells` (`spell_id`, `cat_number`, `cost`, `name`) VALUES
-- Small Buff
(39233,1001,0,"Mark of the Wild"),
(22696,1002,0,"Thorns"),
(33946,1003,0,"Amplify Magic"),
(39235,1004,0,"Arcane Intellect"),
(16888,1005,0,"Dalaran Intellect"),
(33944,1006,0,"Dampen Magic"),
(20217,1007,0,"Blessing of Kings"),
(27140,1008,0,"Blessing of Might"),
(27142,1009,0,"Blessing of Wisdom"),
(39234,1010,0,"Divine Spirit"),
(25389,1011,0,"Power Word: Fortitude"),
(25433,1012,0,"Shadow Protection"),
-- Great Buff
(26991,2001,0,"Gift of the Wild"),
(27127,2002,0,"Arcane Brilliance"),
(469,2003,0,"Commanding Shout"),
(25898,2004,0,"Greater Blessing of Kings"),
(27141,2005,0,"Greater Blessing of Might"),
(27169,2006,0,"Greater Blessing of Sanctuary"),
(27143,2007,0,"Greater Blessing of Wisdom"),
(25392,2008,0,"Prayer of Fortitude"),
(25289,2009,0,"Battle Shout"),
(32999,2010,0,"Prayer of Spirit"),
-- GM Buff
(17013,3001,0,"Agamaggan\s Agility"),
(16612,3002,0,"Agamaggan\s Strength"),
(58453,3003,0,"Armor Magic"),
(34106,3004,0,"Armor Penetration"),
(25661,3005,0,"Increased Stamina"),
(16610,3006,0,"Razorhide"),
(10767,3007,0,"Rising Spirit"),
(16618,3008,0,"Spirit of the Wind"),
(7764,3009,0,"Wisdom of Agamaggan"),
-- Player Tools
(46642,4001,0,"Give me Gold"),
(20372,4002,0,"Tag: Hearts"),
(41386,4003,0,"Heal me please"),
(42027,4004,0,"Heal me and party members Please"),
(42365,4005,0,"Murloc Costume"),
(27101,4006,0,"Conjure Mana Gem");

REPLACE INTO `npc_text` (`ID`, `text0_0`) VALUES
   (100002, 'Choose your Buff Type.'),
   (100003, 'Choose your Buff.');

DELETE FROM `creature_template` WHERE `entry` = 99992;

INSERT INTO `creature_template` (`entry`, `heroic_entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `DamageModifier`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) values
('99992','0','0','0','19646','0','19646','0','Buff NPC','',NULL,'0','70','70','0','5999','5999','1999','1999','2999','35','1','1','1.14286','1','0','0','1500','1500','1','1','1','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','1','1','1','1','1','1','0','1','0','0','0','npc_buff');