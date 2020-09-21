DROP TABLE IF EXISTS `alphaquestpoints`;

CREATE TABLE IF NOT EXISTS `alpha_reward_system` (
  `Entry` int(11) NOT NULL AUTO_INCREMENT,
  `TypeId` int(11) NOT NULL DEFAULT '0' COMMENT '1 = creature 2= quest',
  `Points` int(11) NOT NULL DEFAULT '0',
  `Comment` varchar(255) NOT NULL,
  PRIMARY KEY (`Entry`)
) ENGINE=InnoDB AUTO_INCREMENT=24791 DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `alpha_reward_system_items` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `itemId` int(11) NOT NULL,
  `points` int(11) NOT NULL,
  `quantity` int(11) DEFAULT '1',
  PRIMARY KEY (`id`),
  UNIQUE KEY `itemId` (`itemId`)
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=utf8;


