SET @NPC_ID := ENTER_NPC_ID_HERE;
REPLACE INTO `creature_template` (`entry`, `heroic_entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `minhealth`, `maxhealth`, `minmana`, `maxmana`, `armor`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `class`, `race`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `PetSpellDataId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `DamageModifier`, `RacialLeader`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES (@NPC_ID, '0', '0', '0', '10458', '0', '10458', '0', 'Arena Theme', 'Arena Master', '', '0', '60', '60', '0', '3052', '3052', '0', '0', '0', '35', '1', '1', '1.14286', '1', '0', '0', '0', '0', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '7', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '', '0', '3', '1', '1', '0', '1', '0', '1', '0', '0', '0', 'Theme_NPC');

DELETE FROM `creature` where `id` = @NPC_ID;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES (@NPC_ID, '0', '1', '0', '0', '-13165.3', '213.999', '42.9794', '2.20305', '25', '0', '0', '3052', '0', '0', '0', '0', '0');

REPLACE INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `faction`, `flags`, `size`) VALUES 
(6000113, 5, 28, 'ARENA_bush', 0, 0, 1),
(6000110, 5, 7331, 'ARENA_water', 0, 0, 15),
(6000111, 5, 967, 'ARENA_tree', 0, 0, 1),
(6000114, 5, 7341, 'ARENA_hugetree', 0, 0, 1),
(6000112, 14, 7465, 'ARENA_humanhouse', 0, 0, 1),
(6000118, 14, 7051, 'ARENA_bridge', 0, 0, 3),
(6000115, 5, 6151, 'ARENA_fence', 0, 0, 1),
(6000116, 5, 4315, 'ARENA_rock', 0, 0, 2),
(6000127, 5, 362, 'ARENA_walkstone', 0, 0, 1),
(6000117, 5, 223, 'ARENA_boat', 0, 0, 3),
(6000119, 5, 2315, 'ARENA_grass', 0, 0, 6),
(6000120, 5, 8557, 'ARENA_snow_A', 0, 0, 7),
(6000121, 14, 8557, 'ARENA_snow_B', 0, 0, 20),
(6000122, 5, 927, 'ARENA_snow_tree_1', 0, 0, 1),
(6000123, 5, 7801, 'ARENA_snow_tree_2', 0, 0, 4),
(6000124, 5, 8219, 'ARENA_snow_myst', 0, 0, 20),
(6000125, 5, 7492, 'ARENA_frozenrock', 0, 0, 0.23),
(6000126, 5, 7666, 'ARENA_frostmonolite', 0, 0, 1.5);

-- ----------------------------
-- Table structure for `gurubashi_lastspawned`
-- ----------------------------
DROP TABLE IF EXISTS `gurubashi_lastspawned`;
CREATE TABLE `gurubashi_lastspawned` (
  `time` int(11) default NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for `gurubashi_spawns`
-- ----------------------------
DROP TABLE IF EXISTS `gurubashi_spawns`;
CREATE TABLE `gurubashi_spawns` (
  `theme` int(11) default NULL,
  `x` double default NULL,
  `y` double default NULL,
  `z` double default NULL,
  `o` double default NULL,
  `entry` int(11) default NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of gurubashi_spawns
-- ----------------------------
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13205.8', '274.844', '27.6802', '4.30156', '6000118');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13168', '275.081', '21.8574', '2.72372', '6000117');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13205.5', '274.828', '30.32', '4.27407', '6000110');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13187.7', '239.463', '21.8582', '3.77539', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13180.5', '291.8', '21.8582', '3.22953', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13178.4', '283.759', '21.8579', '4.64874', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13227.1', '278.129', '21.8582', '5.76243', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13173.3', '295.848', '21.8579', '4.20499', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13202.6', '244.986', '21.8579', '2.8816', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13186.4', '291.49', '21.8579', '5.90144', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13192.9', '235.159', '21.8582', '3.5437', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13173.4', '255.883', '21.8582', '4.31731', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13205.5', '250.809', '21.8582', '2.8329', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13218.7', '268.308', '21.8582', '5.81348', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13170.4', '287.805', '21.8582', '2.30276', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13228', '249.127', '21.8582', '5.77814', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13215.8', '312.145', '21.8582', '0.928307', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13180.1', '239.075', '21.8579', '5.35167', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13221.7', '274.7', '21.8571', '4.26782', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13167.4', '256.948', '21.8579', '6.12921', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13170.8', '249.312', '21.8579', '5.68546', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13184.4', '274.314', '21.8582', '5.65248', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13210.4', '283.529', '21.8582', '5.86061', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13198.1', '271.543', '21.8582', '2.64048', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13193.6', '258.863', '21.8582', '2.69153', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13199.2', '314.904', '21.8582', '1.06968', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13203.6', '298.728', '21.8582', '6.15121', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13231.4', '261.383', '21.8571', '4.62517', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13175.7', '274.391', '21.8579', '5.77107', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13177.2', '267.055', '21.8579', '0.591363', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13180.3', '245.738', '21.8582', '3.9521', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13210.4', '302.124', '21.8582', '4.39192', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13233.7', '262.174', '21.8582', '4.16415', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13173.5', '260.887', '21.8582', '5.30691', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13186.7', '282.938', '21.8582', '4.39585', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13242.4', '276.602', '21.8573', '0.159389', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13238.3', '281.879', '21.8573', '0.921225', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13225.4', '300.29', '21.8573', '3.53268', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13214.3', '292.125', '21.8573', '4.3652', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13220.4', '307.884', '21.8573', '1.96581', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('1', '-13176.7', '304.866', '21.8573', '1.06653', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13203.59', '265.7', '21.8', '0', '6000113');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13204.2', '263', '21.8', '0', '6000113');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13201.4', '262.59', '21.8', '0', '6000113');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13211.4', '276.35', '21.8', '0', '6000111');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13185.8', '256.4', '21.8', '0', '6000111');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13202.1', '250.3', '21.8', '0', '6000111');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13174.5', '284.3', '21.8', '0', '6000114');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13190.1', '302.3', '21.8', '0', '6000114');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13215.4', '301.6', '21.8', '0', '6000114');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13194.1', '282.35', '19', '0', '6000112');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13187.7', '239.463', '21.8582', '3.77539', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13180.5', '291.8', '21.8582', '3.22953', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13178.4', '283.759', '21.8579', '4.64874', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13227.1', '278.129', '21.8582', '5.76243', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13173.3', '295.848', '21.8579', '4.20499', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13202.6', '244.986', '21.8579', '2.8816', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13186.4', '291.49', '21.8579', '5.90144', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13192.9', '235.159', '21.8582', '3.5437', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13173.4', '255.883', '21.8582', '4.31731', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13205.5', '250.809', '21.8582', '2.8329', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13218.7', '268.308', '21.8582', '5.81348', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13170.4', '287.805', '21.8582', '2.30276', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13228', '249.127', '21.8582', '5.77814', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13215.8', '312.145', '21.8582', '0.928307', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13180.1', '239.075', '21.8579', '5.35167', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13221.7', '274.7', '21.8571', '4.26782', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13167.4', '256.948', '21.8579', '6.12921', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13170.8', '249.312', '21.8579', '5.68546', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13184.4', '274.314', '21.8582', '5.65248', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13210.4', '283.529', '21.8582', '5.86061', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13198.1', '271.543', '21.8582', '2.64048', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13193.6', '258.863', '21.8582', '2.69153', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13199.2', '314.904', '21.8582', '1.06968', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13203.6', '298.728', '21.8582', '6.15121', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13231.4', '261.383', '21.8571', '4.62517', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13175.7', '274.391', '21.8579', '5.77107', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13177.2', '267.055', '21.8579', '0.591363', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13180.3', '245.738', '21.8582', '3.9521', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13210.4', '302.124', '21.8582', '4.39192', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13233.7', '262.174', '21.8582', '4.16415', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13173.5', '260.887', '21.8582', '5.30691', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13186.7', '282.938', '21.8582', '4.39585', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13242.4', '276.602', '21.8573', '0.159389', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13238.3', '281.879', '21.8573', '0.921225', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13225.4', '300.29', '21.8573', '3.53268', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13214.3', '292.125', '21.8573', '4.3652', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13220.4', '307.884', '21.8573', '1.96581', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('2', '-13176.7', '304.866', '21.8573', '1.06653', '6000119');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-293.268', '-2664.01', '120.55', '4.76707', '6000110');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-297.909', '-2667.51', '127.08', '1.03251', '6000110');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-262.17', '-2645.9', '125.866', '5.60904', '6000111');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-1486.42', '526.784', '142.354', '1.77347', '6000111');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-241.205', '-2648.73', '122.741', '5.38912', '6000112');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-254.937', '-2664.5', '122.403', '0.877', '6000113');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-251.201', '-2667.65', '121.778', '1.17624', '6000114');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-276.437', '-2660.31', '122.567', '4.27856', '6000115');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-271.854', '-2658.65', '122.628', '0.418328', '6000115');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-267.148', '-2655.72', '122.602', '2.31507', '6000116');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-267.712', '-2658.16', '125.609', '4.53539', '6000117');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-272.264', '-2649.73', '122.459', '5.64908', '6000117');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-270.348', '-2652.22', '122.613', '2.31114', '6000117');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-272.47', '-2652.4', '122.674', '3.24184', '6000117');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-238.774', '-2570.31', '163.17', '2.01427', '6000120');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-233.616', '-2587.23', '185.726', '4.83934', '6000120');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13204', '273', '17.4388', '1.50755', '6000120');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-251.025', '-2603.47', '192.756', '0.812609', '6000121');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-241.481', '-2584.59', '185.905', '1.29955', '6000122');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13201.5', '280.86', '25.2712', '3.87555', '6000122');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13207.5', '256.319', '24.8636', '4.47638', '6000122');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13227', '273.394', '24.7136', '2.3558', '6000122');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13214.8', '289.288', '24.8299', '4.74341', '6000122');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-186.58', '-2605.13', '207.157', '2.64021', '6000123');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-218.607', '-2578.01', '196.721', '4.11755', '6000123');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-234.26', '-2600.91', '208.531', '1.80219', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-228.942', '-2594.19', '222.955', '0.675144', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13204.2', '263.16', '25.1665', '1.66072', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13204.3', '268.294', '25.3861', '3.07836', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13214.4', '270.035', '25.1944', '4.25253', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13208.7', '276.679', '25.4071', '1.07559', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13217.1', '278.225', '25.0689', '2.85059', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13224.5', '283.428', '24.7078', '2.54822', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13222.2', '291.453', '24.5588', '0.824266', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13231.4', '262.306', '24.4435', '3.55746', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13233.5', '251.119', '23.8527', '3.70668', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13241', '271.689', '24.0802', '2.93306', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13233.2', '291.237', '24.2631', '2.17908', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13212.3', '303.589', '24.2717', '0.333392', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13212.3', '303.589', '24.2717', '1.25624', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13212.2', '307.054', '24.0447', '1.76675', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13198.8', '308.434', '23.9304', '1.46044', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13185.6', '301.572', '24.2001', '4.5392', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13167.1', '286.922', '23.5056', '2.88987', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13173.5', '278.781', '24.3101', '3.92267', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13176.7', '273.083', '25.0311', '4.80232', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13196.1', '242.097', '24.2017', '4.89264', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13208.6', '244.898', '24.3776', '4.05226', '6000124');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13186.4', '256.64', '24.5668', '2.27724', '6000125');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13182.2', '259.654', '24.5465', '0.180231', '6000125');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13184.4', '286.078', '24.6236', '3.67133', '6000125');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13189', '289.142', '24.6753', '0.781064', '6000125');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13181.8', '291.011', '24.3994', '6.271', '6000125');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-186.979', '-2617', '206.865', '4.86525', '6000126');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13210.4', '279.867', '23.5357', '5.72907', '6000126');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13199.6', '267.771', '23.6485', '2.37149', '6000126');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13216.6', '264.357', '23.4314', '1.00882', '6000126');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-163.572', '-2611.32', '205.179', '1.55714', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13239.3', '261.181', '23.0034', '3.36503', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13236.6', '263.074', '23.2352', '0.612207', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13233.5', '264.871', '23.3779', '0.462981', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13230.2', '266.156', '23.5204', '0.305901', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13227.2', '266.984', '23.6516', '0.246996', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13223.8', '267.906', '23.7959', '0.282339', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13220.3', '268.995', '23.9446', '0.341244', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13216.9', '270.403', '24.0969', '0.415857', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13213.8', '271.861', '24.233', '0.466908', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13210.7', '273.792', '24.3767', '0.290194', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13207.1', '273.777', '24.5207', '6.23959', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13202.6', '273.419', '24.5323', '6.17675', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13198.2', '273.688', '24.3519', '0.140973', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13194.1', '273.484', '24.1867', '5.91365', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13189.8', '272.922', '24.02', '0.160611', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13185.5', '273.637', '23.8414', '0.168465', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13181.5', '274.256', '23.6722', '6.1532', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13176.8', '273.006', '23.4937', '5.96078', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13172', '272.635', '23.3019', '0.0585103', '6000127');
INSERT INTO `gurubashi_spawns` VALUES ('3', '-13167.1', '272.97', '22.9589', '0.0820722', '6000127');

-- ----------------------------
-- Table structure for `gurubashi_themes`
-- ----------------------------
DROP TABLE IF EXISTS `gurubashi_themes`;
CREATE TABLE `gurubashi_themes` (
  `id` int(11) NOT NULL default '0',
  `name` text,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of gurubashi_themes
-- ----------------------------
INSERT INTO `gurubashi_themes` VALUES ('2', 'Elwynn Theme');
INSERT INTO `gurubashi_themes` VALUES ('1', 'Marsh Theme');
INSERT INTO `gurubashi_themes` VALUES ('3', 'Winter Theme');