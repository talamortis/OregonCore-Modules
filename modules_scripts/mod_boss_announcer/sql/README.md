# BEST PRACTICES

## Create 3 folders, realmd, characters, world

for one-time update
create sql files inside those 3 folders, name them as YYYY_MM_DD_COMMENTS.sql
adding query inside those file

core only apply those file once

for config file purpose, name them as YYYY_MM_DD_COMMENTS.conf.sql
core will always re-apply those file so you can edit the config without using sql tools.

you must add delete query, and insert into mod_config table, example as below

delete from `updates` where `update` = '2020_01_17_world_mod_test.conf.sql';
REPLACE INTO `module_config` (`id`, `config`, `value`, `comment`) VALUES
	(1, 'modsample.enableHelloWorld', '0', NULL),
	(2, 'modsample.stringtest', 'String is working! :D', NULL),
	(3, 'modsample.intTest', '1908', NULL);

## Resources

https://www.w3schools.com/sql/sql_datatypes.asp
