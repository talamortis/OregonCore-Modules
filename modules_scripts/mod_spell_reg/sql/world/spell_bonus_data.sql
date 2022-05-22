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

-- Dumping structure for table mangos.spell_bonus_data
CREATE TABLE IF NOT EXISTS `spell_bonus_data` (
  `entry` int(11) NOT NULL,
  `direct_bonus` float NOT NULL DEFAULT '1',
  `dot_bonus` float NOT NULL DEFAULT '1',
  `ap_bonus` float NOT NULL DEFAULT '1',
  `dot_ap_bonus` float NOT NULL DEFAULT '1',
  `direct_bonus_player` float NOT NULL DEFAULT '1',
  `dot_bonus_player` float NOT NULL DEFAULT '1',
  `ap_bonus_player` float NOT NULL DEFAULT '1',
  `dot_ap_bonus_player` float NOT NULL DEFAULT '1',
  `comments` varchar(255) COLLATE armscii8_bin NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=armscii8 COLLATE=armscii8_bin;

-- Data exporting was unselected.

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
