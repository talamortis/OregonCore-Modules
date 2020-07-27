-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.6.36-log - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL Version:             9.4.0.5125
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- Dumping structure for table acore_characters.chat_censure
CREATE TABLE IF NOT EXISTS `chat_censure` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `text` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=69 DEFAULT CHARSET=latin1;

-- Dumping data for table acore_characters.chat_censure: ~0 rows (approximately)
DELETE FROM `chat_censure`;
/*!40000 ALTER TABLE `chat_censure` DISABLE KEYS */;
INSERT INTO `chat_censure` (`id`, `text`) VALUES
	(1, 'http://'),
	(2, 'sytes'),
	(3, 'https://'),
	(4, 'h t t p : / /'),
	(5, 'h t t p s : / /'),
	(6, '.com'),
	(7, '.net'),
	(8, '.org'),
	(9, 'www.'),
	(10, 'wow-'),
	(11, 'wow'),
	(12, 'no-ip'),
	(13, 'zapto'),
	(14, '.biz'),
	(15, '.servegame'),
	(16, '.br'),
	(17, 'com.br'),
	(18, '. c o m'),
	(19, '. n e t'),
	(20, '. o r g'),
	(21, 'w w w .'),
	(22, ' w o w -'),
	(23, '- w o w'),
	(24, 'n o - i p'),
	(25, '. z a p t o'),
	(26, '. b i z'),
	(27, '. s e r v e g a m e'),
	(28, '. b r'),
	(29, 'c o m . b r'),
	(30, 'h  t  t  p  :  /  /'),
	(31, '.  c  o  m'),
	(32, '.  n  e  t'),
	(33, '.  o  r  g'),
	(34, 'w  w  w  .'),
	(35, ' w  o  w  -'),
	(36, '-  w  o  w'),
	(37, 'n  o  -  i  p'),
	(38, '.  z  a  p  t  o'),
	(39, '.  b  i  z'),
	(40, '.  s  e  r  v  e  g  a  m  e'),
	(41, '.  b  r'),
	(42, 'c  o  m  .  b  r'),
	(43, 'h   t   t   p   :   /   /'),
	(44, '.   c   o   m'),
	(45, '.   n   e   t'),
	(46, '.   o   r   g'),
	(47, 'w   w   w   .'),
	(48, ' w   o   w   -'),
	(49, '-   w   o   w'),
	(50, 'n   o   -   i   p'),
	(51, '.   z   a   p   t   o'),
	(52, '.   b   i   z'),
	(53, '.   s   e   r   v   e   g   a   m   e'),
	(54, '.   b   r'),
	(55, '   c   o   m   .   b   r'),
	(56, 'h    t    t    p   :   /   /'),
	(57, '.    c    o    m'),
	(58, '.    n    e   t'),
	(59, '.    o    r    g'),
	(60, 'w    w    w    .'),
	(61, 'w    o    w    -'),
	(62, '-    w    o    w'),
	(63, 'n    o    -    i    p'),
	(64, '.    z    a    p    t    o'),
	(65, '.    b    i     z'),
	(66, '.    s    e    r    v    e    g    a    m    e'),
	(67, '.    b    r'),
	(68, 'c    o    m    .    b    r');
/*!40000 ALTER TABLE `chat_censure` ENABLE KEYS */;

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
