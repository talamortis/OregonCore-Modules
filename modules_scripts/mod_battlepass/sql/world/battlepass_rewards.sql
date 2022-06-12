-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.7.36-log - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL Version:             11.3.0.6295
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

-- Dumping structure for table mangos.battlepass_rewards
CREATE TABLE IF NOT EXISTS `battlepass_rewards` (
  `id` int(11) NOT NULL,
  `option` int(11) DEFAULT NULL,
  `reward` text COLLATE armscii8_bin,
  `amount` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=armscii8 COLLATE=armscii8_bin COMMENT='In the option\r\n1 = Item\r\n2 = Gold\r\n3 = level';

-- Data exporting was unselected.

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;


  `reward` int(11) DEFAULT NULL,
  `amount` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=armscii8 COLLATE=armscii8_bin COMMENT='In the option\r\n1 = Item\r\n2 = Gold\r\n3 = level';

DELETE FROM `module_config` WHERE `config` = 'BattlePassLevelPvP';
DELETE FROM `module_config` WHERE `config` = 'BattlePassLevelQuest';
INSERT INTO `module_config` (`config`, `value`, `comment`) VALUES ('BattlePassLevelPvP', '20', NULL);
INSERT INTO `module_config` (`config`, `value`, `comment`) VALUES ('BattlePassLevelQuest', '10', NULL);