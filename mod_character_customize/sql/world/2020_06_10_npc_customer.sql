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
