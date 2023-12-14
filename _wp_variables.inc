<?php
/*
 * WordPress用の変数
 *
 * © LEANSYSTEM
 * Contact: nabe@lean-system.co.jp
 *
 * Figma・XDでのAnimaの使用方法やプログラムについてのご質問、ご相談などありましたらお気軽にお問い合わせください。
 *
 */

// ホームURL
if(function_exists('home_url')) {
  $home_uri = esc_url( home_url( '/' ) );
} else {
  // wordpressを使用しない場合
  $home_uri = '.';
}

// themeディレクトリまでのパス
if(function_exists('get_template_directory_uri')) {
  $theme_uri = get_template_directory_uri();
} else {
  // wordpressを使用しない場合は空
  $theme_uri = '.';
}

// wp_headを変数化
if(function_exists('wp_head')) {
  ob_start();
  wp_head();
  $wp_head = ob_get_clean();
} else {
  // wordpressを使用しない場合は空
  $wp_head = '';
}

// wp_footerを変数化
if(function_exists('wp_footer')) {
  ob_start();
  wp_footer();
  $wp_footer = ob_get_clean();
} else {
  // wordpressを使用しない場合は空
  $wp_footer = '';
}