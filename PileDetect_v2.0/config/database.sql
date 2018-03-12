/*
SQLITE Backup
Source Server Version: 3.12.2
Source Database: main
Date: 2018/2/1 18:46:15
*/


-- ----------------------------
--  Table structure for "circles"
-- ----------------------------
DROP TABLE IF EXISTS "circles";
CREATE TABLE "circles" (
"id"  INTEGER NOT NULL,
"circle_name"  TEXT NOT NULL,
"x"  REAL NOT NULL,
"y"  REAL NOT NULL,
"radius"  REAL NOT NULL,
"gx"  REAL,
"gy"  REAL,
"gr"  REAL,
"error_gx"  REAL,
"error_gy"  REAL,
"error_gr"  REAL,
PRIMARY KEY ("id" ASC),
UNIQUE ("circle_name") ON CONFLICT IGNORE
);

-- ----------------------------
--  Table structure for "correct_mode"
-- ----------------------------
DROP TABLE IF EXISTS "correct_mode";
CREATE TABLE "correct_mode" (
"geoCorrectMode"  INTEGER NOT NULL DEFAULT 0
);

-- ----------------------------
--  Table structure for "cpoints"
-- ----------------------------
DROP TABLE IF EXISTS "cpoints";
CREATE TABLE "cpoints" (
"id"  INTEGER NOT NULL,
"point_name"  TEXT NOT NULL,
"x"  REAL NOT NULL,
"y"  REAL NOT NULL,
PRIMARY KEY ("id" ASC),
UNIQUE ("point_name") ON CONFLICT REPLACE
);

-- ----------------------------
--  Table structure for "project"
-- ----------------------------
DROP TABLE IF EXISTS "project";
CREATE TABLE "project" (
"id"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"image"  TEXT NOT NULL,
"state_point"  INTEGER NOT NULL DEFAULT 0,
"state_circle"  INTEGER NOT NULL DEFAULT 0,
"parms_m1"  REAL,
"parms_m2"  REAL,
"parms_x0"  REAL,
"parms_y0"  REAL,
"p1_x"  REAL,
"p1_y"  REAL,
"p2_x"  REAL,
"p2_y"  REAL,
"p3_x"  REAL,
"p3_y"  REAL,
"p4_x"  REAL,
"p4_y"  REAL,
"a1"  REAL,
"a2"  REAL,
"a3"  REAL,
"a4"  REAL,
"a5"  REAL,
"a6"  REAL,
"b1"  REAL,
"b2"  REAL,
"b3"  REAL,
"b4"  REAL,
"b5"  REAL,
"b6"  REAL,
"a1_inv"  REAL,
"a2_inv"  REAL,
"a3_inv"  REAL,
"a4_inv"  REAL,
"a5_inv"  REAL,
"a6_inv"  REAL,
"b1_inv"  REAL,
"b2_inv"  REAL,
"b3_inv"  REAL,
"b4_inv"  REAL,
"b5_inv"  REAL,
"b6_inv"  REAL,
"c1"  REAL,
"c2"  REAL,
"c3"  REAL,
"c4"  REAL,
"c5"  REAL,
"c6"  REAL,
"avg_pe"  REAL,
"mode"  INTEGER NOT NULL DEFAULT 0,
UNIQUE ("image" ASC) ON CONFLICT IGNORE
);
INSERT INTO sqlite_sequence VALUES ("project",245);

-- ----------------------------
--  Table structure for "select_circles"
-- ----------------------------
DROP TABLE IF EXISTS "select_circles";
CREATE TABLE "select_circles" (
"id"  INTEGER NOT NULL,
"image_id"  INTEGER NOT NULL,
"circle_id"  INTEGER NOT NULL,
"clc_px"  REAL NOT NULL,
"clc_py"  REAL NOT NULL,
"clc_pr"  REAL NOT NULL,
"sel_px"  REAL,
"sel_py"  REAL,
"sel_pr"  REAL,
"sel_gx"  REAL,
"sel_gy"  REAL,
"sel_gr"  REAL,
"g_error"  REAL,
PRIMARY KEY ("id" ASC),
CONSTRAINT "fkey0" FOREIGN KEY ("image_id") REFERENCES "project" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
CONSTRAINT "fkey1" FOREIGN KEY ("circle_id") REFERENCES "circles" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
CONSTRAINT "unique_key" UNIQUE ("image_id" ASC, "circle_id" ASC) ON CONFLICT REPLACE
);

-- ----------------------------
--  Table structure for "select_cpoints"
-- ----------------------------
DROP TABLE IF EXISTS "select_cpoints";
CREATE TABLE "select_cpoints" (
"id"  INTEGER NOT NULL,
"image_id"  INTEGER NOT NULL,
"cpoint_id"  INTEGER NOT NULL,
"px"  REAL,
"py"  REAL,
"p_error"  REAL,
"g_error"  REAL,
PRIMARY KEY ("id" ASC),
CONSTRAINT "fkey0" FOREIGN KEY ("image_id") REFERENCES "project" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
CONSTRAINT "fkey1" FOREIGN KEY ("cpoint_id") REFERENCES "cpoints" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
CONSTRAINT "unique_select" UNIQUE ("image_id" ASC, "cpoint_id" ASC) ON CONFLICT REPLACE
);

-- ----------------------------
--  View definition for "collect_circles"
-- ----------------------------
DROP VIEW IF EXISTS "collect_circles";
CREATE VIEW "collect_circles" AS 
SELECT DISTINCT
select_circles.circle_id,
select_circles.sel_gx AS gx,
select_circles.sel_gy AS gy,
select_circles.sel_gr AS gr,
Min(select_circles.g_error)
FROM
project
INNER JOIN select_circles ON select_circles.image_id = project.id
WHERE
project.state_circle = 1
GROUP BY
select_circles.circle_id;

-- ----------------------------
--  View definition for "select_circles_with_names"
-- ----------------------------
DROP VIEW IF EXISTS "select_circles_with_names";
CREATE VIEW "select_circles_with_names" AS 
SELECT
select_circles.image_id,
select_circles.circle_id,
circles.circle_name,
select_circles.g_error,
select_circles.clc_px,
select_circles.clc_py,
select_circles.clc_pr,
select_circles.sel_px,
select_circles.sel_py,
select_circles.sel_pr
FROM
circles
INNER JOIN select_circles ON select_circles.circle_id = circles.id;

-- ----------------------------
--  View definition for "select_cpoints_with_names"
-- ----------------------------
DROP VIEW IF EXISTS "select_cpoints_with_names";
CREATE VIEW "select_cpoints_with_names" AS 
SELECT
select_cpoints.px,
select_cpoints.py,
select_cpoints.p_error,
select_cpoints.g_error,
cpoints.point_name AS cpoint_name,
select_cpoints.cpoint_id,
select_cpoints.image_id
FROM
cpoints
INNER JOIN select_cpoints ON select_cpoints.cpoint_id = cpoints.id;

-- ----------------------------
--  Records 
-- ----------------------------
INSERT INTO "correct_mode" VALUES (0);
-- ----------------------------
--  Trigger definition for "updatex"
-- ----------------------------
DROP TRIGGER IF EXISTS "updatex";
CREATE TRIGGER "updatex" AFTER UPDATE ON "circles"
BEGIN
    UPDATE circles SET error_gx= new.x-new.gx WHERE id= old.id;
END;

-- ----------------------------
--  Trigger definition for "updatey"
-- ----------------------------
DROP TRIGGER IF EXISTS "updatey";
CREATE TRIGGER "updatey" AFTER UPDATE ON "circles"
BEGIN
    UPDATE circles SET error_gy= new.y-new.gy WHERE id= old.id;
END;

-- ----------------------------
--  Trigger definition for "updater"
-- ----------------------------
DROP TRIGGER IF EXISTS "updater";
CREATE TRIGGER "updater" AFTER UPDATE ON "circles"
BEGIN
    UPDATE circles SET error_gr= new.gr-new.radius WHERE id= old.id;
END;
