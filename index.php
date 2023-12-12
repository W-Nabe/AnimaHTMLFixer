<?php
// error_reporting(0); // デバッグ時はこの行をコメントアウト
require_once("_wp_variables.inc");
require_once("_check-ua.inc"); // ブラウザ横幅のチェックとDOCTYPE,html,headタグ配置

if (isset($_COOKIE["windowWidth"])) require_once("./_operate_views.inc");
?>
