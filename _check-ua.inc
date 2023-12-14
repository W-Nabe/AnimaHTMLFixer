<?php
/*
 * User-Agentを得てレンダーする
 *
 * © LEANSYSTEM
 * Contact: nabe@lean-system.co.jp
 *
 * Figma・XDでのAnimaの使用方法やプログラムについてのご質問、ご相談などありましたらお気軽にお問い合わせください。
 * 
 */
?>
<!DOCTYPE html>
<html lang="ja">
<head>
<?php
// User-AgentからポータブルデバイスかPCかを判断する。
function ua_smt () {
  //ユーザーエージェントを取得
  $ua = $_SERVER['HTTP_USER_AGENT'];
  //スマホと判定する文字リスト
  $ua_list = array('iPhone','iPad','iPod','Android');
  foreach ($ua_list as $ua_smt) {
    //ユーザーエージェントに文字リストの単語を含む場合はTRUE、それ以外はFALSE
    if (strpos($ua, $ua_smt) !== false) {
     return true;
    }
  }

  return false;
}

$config = array();
$handle = fopen(__DIR__ ."/anima/config/config.csv", "r");
if (!$handle) exit;

fgetcsv($handle); // タイトル行をスキップ
while ($data = fgetcsv($handle)) {
  if ($data[0] == "spbreakpoint") {
    $spbreakpoint = $data[2];
    break;
  }
}
fclose($handle);

if (ua_smt()) {
  // スマホ表示
  $_COOKIE["windowWidth"] = $spbreakpoint - 1;
} else {
  // PC表示
  $_COOKIE["windowWidth"] = $spbreakpoint;
  // $_COOKIE["windowWidth"] = 100; // PCでもスマホ表示したい場合はコメントを外す
}
?>
