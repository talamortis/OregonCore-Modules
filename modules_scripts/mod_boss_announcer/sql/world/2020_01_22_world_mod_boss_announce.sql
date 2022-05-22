delete from `updates` where `update` = '2020_01_22_world_mod_boss_announce.sql';
INSERT INTO `module_config` (`config`, `value`, `comment`) VALUES
	('Boss.Announcer.Enable', '1', NULL),
	('Boss.Announcer.Announce', '1', NULL),
	('Boss.Announcer.RemoveAuraUponKill', '1', NULL);