DROP TABLE IF EXISTS `beastmaster`;

CREATE TABLE `beastmaster` (
  `entry` INT(5) NOT NULL DEFAULT '0' COMMENT 'Pet entry number',
  `cat_number` INT(5) NOT NULL DEFAULT '0' COMMENT '0-normal pet 1-exotic pet 2-pet spell',
  `tokenOrGold` TINYINT(1) DEFAULT '0' COMMENT '0 = gold 1 = token',
  `cost` INT(10) NOT NULL DEFAULT '0' COMMENT 'Amount in copper if tokenOrGold is 0 else number of tokens',
  `token` INT(10) NOT NULL DEFAULT '0' COMMENT 'Token entry number (item_template.entry)',
  `name` VARCHAR(255) DEFAULT NULL COMMENT 'Name of pet or name of pet spell',
  `spell` INT(10) NOT NULL DEFAULT '0' COMMENT 'Spell entry number.',
  PRIMARY KEY (`entry`)
) ENGINE=INNODB DEFAULT CHARSET=utf8 COMMENT='Beastmaster System by LordPsyan';

/*Data for the table `beastmaster` */

INSERT INTO `beastmaster`(`entry`,`cat_number`,`tokenOrGold`,`cost`,`token`,`name`,`spell`) values 
(1,0,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Spider:26:26:-22|tSpider',2349),
(2,0,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Bat:26:26:-22|tBat',1512),
(3,0,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Bear:26:26:-22|tBear',1129),
(4,0,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Boar:26:26:-22|tBoar',454),
(5,0,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Cat:26:26:-22|tCat',1201),
(6,0,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Vulture:26:26:-22|tCarrion Bird',26019),
(7,0,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Crab:26:26:-22|tCrab',3106),
(8,0,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Crocolisk:26:26:-22|tCrocolisk',1417),
(9,0,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Raptor:26:26:-22|tRaptor',14821),
(10,0,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Gorilla:26:26:-22|tGorilla',1557),
(11,0,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Hyena:26:26:-22|tHyena',4688),
(12,0,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Owl:26:26:-22|tOwl',23136),
(13,0,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Ravager:26:26:-22|tRaveger',17199),
(20,1,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Chimera:28:28:-22|tChimaera',21879),
(21,1,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_CoreHound:28:28:-22|tCore Hound',21108),
(22,1,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Devilsaur:28:28:-22|tDevilsaur',20931),
(23,1,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Rhino:28:28:-22|tRhino',30445),
(24,1,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Silithid:28:28:-22|tSilithid',5460),
(25,1,0,0,0,'|TInterface/ICONS/Ability_Hunter_Pet_Worm:28:28:-22|tWorm',30148),
(26,1,0,0,0,'|TInterface/ICONS/Ability_Hunter_SeparationAnxiety:28:28:-22|tLoque\'nahak',32517),
(27,1,0,0,0,'|TInterface/ICONS/Ability_Hunter_SeparationAnxiety:28:28:-22|tSkoll',35189),
(28,1,0,0,0,'|TInterface/ICONS/Ability_Hunter_SeparationAnxiety:28:28:-22|tGondria',33776);

-- Beastmaster NPC

DELETE FROM `creature_template` WHERE `entry` = 99990;

INSERT INTO `creature_template` (`entry`, `heroic_entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `DamageModifier`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) values
('99990','0','0','0','11804','0','11804','0','Henry','Beastmaster',NULL,'10001','70','70','0','5999','5999','1999','1999','2999','35','1','1','1.14286','1','0','0','1500','1500','1','1','1','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','1','1','1','1','1','1','0','1','0','0','0','Npc_Beastmaster');

-- Sample npc vendor data for beastmaster

DELETE FROM `npc_vendor` WHERE `entry` = 99990;

INSERT INTO `npc_vendor` (`entry`, `item`, `maxcount`, `incrtime`, `ExtendedCost`) VALUES
('99990','4540','0','0','0'),
('99990','4541','0','0','0'),
('99990','4542','0','0','0');
