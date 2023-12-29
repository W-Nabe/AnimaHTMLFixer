<?php
// ini_set( "display_errors", 1 ); // エラーを強制的に表示したいときはこの行をコメントアウト
// error_reporting(E_ALL); // エラーを強制的に表示したいときはこの行をコメントアウト
// error_reporting(0); // エラーを強制的に非表示したいときはこの行をコメントアウト
require_once("_wp_variables.inc");
require_once("_check-ua.inc"); // ブラウザ横幅のチェックとDOCTYPE,html,headタグ配置

if (isset($_COOKIE["windowWidth"])) require_once("./_operate_views.inc");
// DOMをhtmlとして表示
echo output($dom);
?>
