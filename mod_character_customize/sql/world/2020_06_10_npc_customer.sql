SET @NPC_ID := 999999;
INSERT INTO `creature_template` (`entry`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `minlevel`, `maxlevel`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction`, `npcflag`, `speed_run`, `scale`, `rank`, `dmgschool`, `baseattacktime`, `rangeattacktime`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `type`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `ScriptName`) VALUES 
(@NPC_ID, 22957, 0, 22957, 0, 'Customer', 'OregonCore', '', 70, 70, 5000, 5000, 0, 0, 5000, 35, 1, 1.125, 1, 4, 0, 1600, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 0, 1, 0, 0, 'npc_customizer');

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `npc_customer`
-- ----------------------------
DROP TABLE IF EXISTS `npc_customer`;
CREATE TABLE `npc_customer` (
  `type` int(3) unsigned NOT NULL DEFAULT '0',
  `active` int(3) unsigned NOT NULL DEFAULT '0',
  `text` char(100) NOT NULL,
  `cost_gold` int(11) unsigned NOT NULL DEFAULT '0',
  `cost_honor` int(11) unsigned NOT NULL DEFAULT '0',
  `cost_ap` int(11) unsigned NOT NULL DEFAULT '0',
  `cost_itemid` int(11) unsigned NOT NULL DEFAULT '0',
  `constitemcount` int(11) unsigned NOT NULL DEFAULT '0',
  `action` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of npc_customer
-- ----------------------------
INSERT INTO `npc_customer` VALUES ('1', '1', 'Change My Race', '0', '0', '0', '0', '0', '1001');
INSERT INTO `npc_customer` VALUES ('2', '1', 'Customize Character', '0', '0', '0', '0', '0', '1002');
INSERT INTO `npc_customer` VALUES ('3', '1', 'Change My Name. It kick You.', '0', '0', '0', '0', '0', '1003');
