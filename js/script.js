/*
Auth:LEANSYSTEM
Ver:1.0.0
※バージョン書き方：大きな変更.追加機能.修正

【note】
★JSの読み込み
Animaで書き出したファイルの<body>直後に以下を記述
  <script src="js/script.js"></script>

★lazyload
Animaで書き出したファイルの「src="img/」を「data-original="img/」へ置換
※XD上で、PCは「pc-wrap」、タブレットは「tablet-wrap」、スマホは「sp-wrap」という名前のグループをトップの階層に作ること
※XD上で、スクロールでついてくる要素はレイヤー・グループ名にfixed-を付ける

★Slick
※XD上でグループ名に「slick」と記述。「slick-show2」とすると、2つ表示（4つまで設定可能）

*/
// ************************************** //
// **定数 定義
const PC_BREAKPOINT = 1062;
const TABLET_BREAKPOINT = 768;
const USE_SATORI = true; // SATORI使用／不使用
const USE_SLICK = true; // SLICK使用／不使用

// ************************************** //
// **変数 定義
var is_pc = false; // PC表示識別
var is_tablet = false; // タブレット表示識別
var is_sp = false; // スマホ表示識別

var content_pc = 0; // PCコンテンツ保存用
var content_tablet = 0; // タブレットコンテンツ保存用
var content_sp = 0; // スマホコンテンツ保存用

var window_width = document.documentElement.clientWidth || document.body.clientWidth; // 2021/8/31ipadで拡大するとリサイズされるため先頭の「window.innerWidth || 」を削除

var url = window.location; // 現在ページのURL
var file_ex_name = url.href.split('/').pop().replace(/#.*$/,""); if(file_ex_name == "") file_ex_name = "index.html"; // 現在表示中の拡張子付きHTMLファイル名
var file_name = file_ex_name.split('.')[0]; // 現在表示中のHTMLファイル名

// ************************************** //
// **JS読み込み
if(USE_SLICK == true) $('body').append('<script type="text/javascript" src="//cdn.jsdelivr.net/npm/slick-carousel@1.8.1/slick/slick.min.js"></script>');
$('body').append('<script src="js/lazyload/jquery.lazyload.min.js"></script>'); //※lazyloadライブラリの$self.hide();と$self[settings.effect](settings.effect_speed);を削除
setTimeout(() => { // すぐに使わないため、遅延読み込み
  $('body').append('<script src="//cdnjs.cloudflare.com/ajax/libs/jquery.perfect-scrollbar/1.5.2/perfect-scrollbar.min.js"></script>');
  $('head').append('<link href="//cdnjs.cloudflare.com/ajax/libs/jquery.perfect-scrollbar/1.5.2/css/perfect-scrollbar.min.css" rel="stylesheet">');
  setTimeout(() => {
    var ps = new PerfectScrollbar('.wrap-overlay > .content > .inner');
  },500);
},3000);

// ************************************** //
// **ロード画面
$('body').prepend('<div id="loading" style="position:fixed;width:100%;height:100%;background:#fff;left:0;top:0;z-index:1000;"></div>');
var loading = $('#loading');
var loading_css = (function(param) {return param[0].replace(/\n|\r/g, "");})`
<style>
  #loading {
    transition:all 1s ease;
    opacity: 1;
    background: #fff;
  }
  #loading.hide {
    opacity: 0;
    pointer-events:none;
  }
  .loading-img-wrap {
    transition:all 1s ease;
    opacity: 0;
  }
  .loading-img-wrap.show {
    opacity: 1;
  }
  .loading-img {
    position: absolute;
    z-index: 1;
    top: 50%;
    left: 50%;
    transform: translate(-50%,-50%);
    opacity: .2;
  }
  .progress {
    position: absolute;
    z-index: 2;
    top: 50%;
    left: 50%;
    transform: translate(-50%,-50%);
  }
  .progress .loading-img {
    position: absolute;
    bottom: 0;
    left: 0;
    top: auto;
    transform: none;
    opacity: 1;
  }
  .progress div {
    position: absolute;
    bottom: 0;
    overflow: hidden;
    height: 0%;
    width: 100%;
    transition: 1.5s all ease;
  }
</style>`;
$('head').append(loading_css);
loading.append('<div class="loading-img-wrap"><img class="loading-img bg" src="js/loading/loading.png" srcset="js/loading/loading.png 1x, js/loading/loading@2x.png 2x"><div class="progress"><div><img class="loading-img" src="js/loading/loading.png" srcset="js/loading/loading.png 1x, js/loading/loading@2x.png 2x"></div></div></div>');
setTimeout(() => {$('.loading-img-wrap').addClass('show');},500);
setTimeout(() => {$('.progress').height($('.loading-img.bg').height()).width($('.loading-img.bg').width());},500);

// func.ロード画面
var loading = function(device, html) {
  $('#loading').removeClass('hide');
  $('#loading .progress div').height(0);
  $('#loading,#loading .progress div').css({'transition':'none'});
  setTimeout(() => {$('#loading,#loading .progress div').css({'transition':''});},1);
  var loadCount = 0, //loading状況の初期化
      imgLength = $('[class^="'+device+'-wrap"] > div').first().find('img').length; //読み込む画像の数を取得
      $('[class^="'+device+'-wrap"] > div').first().find('img').each(function() {
      var src = $(this).attr("data-original");
      $("<img>")
          .attr("src", src)
          .on('load', function(){
              loadCount++; //画像が読み込まれたら、loading状況を更新
          });
  });

  var timer = setInterval(function() { //一定間隔でloading状況をローディングバーに反映
      $("#loading .progress div").css({
          "height": (loadCount / imgLength) * 100 + "%" //読み込まれた画像の数を画像全体で割り、%としてローディングバーのwidthに設定
      });
      loadCount += imgLength / 300;
      if((loadCount / imgLength) * 100 >= 100){ //100%読み込まれたらローディングバーを隠す
          clearInterval(timer);
          setTimeout(() => {
            $("#loading").addClass('hide');
            $('[class^="'+ device +'-wrap"]').append(html);
            $('.screen, [class^="'+ device +'-wrap"]').has('[class*="relative"]').addClass("height-auto");
            if(USE_SLICK == true) slick(device);
          },1500);
      }
  }, 100);
};

// ************************************** //
// **lazyload
// func.lazyload
var lazyload = function(device, failure_limit = 20, threshold = 2000) {
  $('[class^="'+device+'-wrap"]').parent('.screen').find('img').lazyload({
    placeholder: 'js/lazyload/dummy.png',
    failure_limit: failure_limit,
    threshold : threshold,
    skip_invisible: false,
  });
  // スクロール時に付いてくる要素（fixed-クラスが付いているもの）読み込み
  $('[class^="'+device+'-wrap"]').parent('.screen').find('[class^="fixed-"] img, img[class^="fixed-"]').lazyload({
    threshold : 99999
  });
}
var lazyload_order = function(device, failure_limit = 20) {
  $('[class^="'+device+'-wrap"] > img').lazyload({
    placeholder: 'js/lazyload/dummy.png',
    failure_limit: failure_limit,
    threshold : 99999,
    skip_invisible: false,
  });
  $('[class^="'+device+'-wrap"]').children('div').each(function(k,v){
    var $this = $(this);
    setTimeout(() => {
      $this.find('img').lazyload({
        placeholder: 'js/lazyload/dummy.png',
        failure_limit: failure_limit,
        threshold : 99999,
        skip_invisible: false,
      });
    }, 1000*k);
  });
}

// ************************************** //
// **Slick
if(USE_SLICK == true) {
  var slick_css = (function(param) {return param[0].replace(/\n|\r/g, "");})`
  <link rel="stylesheet" type="text/css" href="//cdn.jsdelivr.net/npm/slick-carousel@1.8.1/slick/slick.css">
  <link rel="stylesheet" type="text/css" href="//cdn.jsdelivr.net/npm/slick-carousel@1.8.1/slick/slick-theme.css"></link>
  <style>
    .slick-arrows {
      display: block;
      position: absolute;
      top: 50%;
      transform: translate(0, -50%);
      cursor: pointer;
      width: 45px;
      height: 30px;
      background: url('js/slick/slick-arrow.svg') no-repeat center center/100% 100%;
    }
    [class^="sp-wrap"] .slick-arrows {
      width: 35px;
      height: 23px;
    }
    .slick-arrows.prev {
      left: -68px;
      transform: rotate(-90deg);
    }
    .slick-arrows.next {
      right: -68px;
      transform: rotate(90deg);
    }
    [class^="sp-wrap"] .slick-arrows.prev {
      left: -42px;
    }
    [class^="sp-wrap"] .slick-arrows.next {
      right: -42px;
    }
    .slick-dots {
      bottom: -40px !important;
    }
    .slick-dots li {
      margin: 0 2px;
    }
    .slick-list .slick-slide {
      margin:0 15px;
    }
    .slick-list .slick-slide:focus {
      outline: none;
    }
    .slick-list {
      margin:0 -15px;
    }
  </style>`;
  $('head').append(slick_css);

  // funk.Slick
  var slick = function(device) {
    $('[class^="'+device+'-wrap"] [class^="slick-"]').each(function() {
      $(this).find('img').each(function() {
        $(this).attr("src", $(this).attr("data-original")).removeClass(function(index, className) {
          return (className.match(/\bslide-\S+/g) || []).join(' ');
        });
      });

      v = $(this).attr('class');
      switch (true) {
        case /show1/.test(v):
          show = 1;
          break;
        case /show2/.test(v):
          show = 2;
          break;
        case /show3/.test(v):
          show = 3;
          break;
        case /show4/.test(v):
          show = 4;
          break;
        default:
          show = 1;
          break;
      }

      if($(this).hasClass('slick-initialized')) {
        $(this).slick('unslick');
      }
      
      setTimeout(() => {
        $(this).slick({
          prevArrow: '<span class="slick-arrows prev"></span>',
          nextArrow: '<span class="slick-arrows next"></span>',
          dots: true,
          infinite: true,
          cssEase: 'ease',
          speed: 1000,
          slidesToShow: show,
          adaptiveHeight: true
        });
      }, 100);
    });
  };
}

// ************************************** //
// **Overlay
$('body').append('<div class="wrap-overlay"><div class="content"><div class="inner"><div class="insert"></div></div><div class="close"></div></div><div class="close-area"></div></div>');
var overlay_css = (function(param) {return param[0].replace(/\n|\r/g, "");})`
<style>
  .noscroll {
    height: 100vh;
    overflow: hidden;
  }
  .wrap-overlay {
    pointer-events:none;
    opacity: 0;
    background: rgba(0,0,0,.5);
    position: fixed;
    width: 100%;
    height: 100%;
    top: 0;
    left: 0;
    transition: all .2s ease;
    transition-delay: .2s;
    z-index: 9999;
  }
  .wrap-overlay.show {
    pointer-events:auto;
    opacity: 1;
  }
  .wrap-overlay > .close-area {
    position: absolute;
    width: 100%;
    height: 100%;
    top: 0;
    left: 0;
    z-index: 1;
    cursor: pointer;
  }
  .wrap-overlay > .content {
    position: absolute;
    top: 50% !important;
    left: 50% !important;
    transform: translate(-50%, calc(-50% - 15px)) !important;
    z-index: 2;
  }
  .wrap-overlay > .content.html {
    padding: 30px;
    min-width: calc(100vw - 300px);
    box-sizing: border-box;
    transition: .2s all ease;
  }
  .wrap-overlay > .content.html > .inner {
    max-height: calc(100vh - 140px);
    position: relative;
  }
  @media screen and (max-width: 768px) {
    .wrap-overlay > .content.html {
      min-width: calc(100vw - 80px);
    }
  }
  .wrap-overlay > .content > .inner {
    max-width: calc(100vw - 80px);
    max-height: calc(100vh - 80px);
    overflow: hidden;
    transition: all .5s ease;
    position: relative;
  }
  .wrap-overlay > .content > .close {
    position: absolute;
    bottom: -35px !important;
    left: 50% !important;
    transform: translate(-50%, 0) !important;
    height: 15px;
    width: 87px;
    background: url('js/overlay/close.png') no-repeat center center/100% 100%;
    pointer-events: none;
  }
  [class^="overlay-"] {
    cursor: pointer;
  }
  .wrap-overlay > .content.html h1 {
    border-left: 5px #005050 solid;
    line-height: 30px;
    height: 30px;
    font-size: 18px;
    padding-left: 32px;
    margin-bottom: 20px;
    letter-spacing: 0.1rem;
    font-weight: bold;
  }
  .wrap-overlay > .content.html h2 {
    line-height: 30px;
    height: 30px;
    font-size: 18px;
    color: #005050;
    padding-left: 0;
    margin-bottom: 20px;
    letter-spacing: 0.1rem;
    font-weight: bold;
  }
  .wrap-overlay > .content.html h3 {
    line-height: 24px;
    height: 24px;
    font-size: 16px;
    color: #000;
    padding-left: 0;
    margin-bottom: 20px;
    letter-spacing: 0.1rem;
    font-weight: bold;
  }
  .wrap-overlay > .content.html h4 {
    line-height: 24px;
    height: 24px;
    font-size: 16px;
    color: #000;
    padding-left: 0;
    margin-bottom: 10px;
    letter-spacing: 0.1rem;
    font-weight: bold;
  }
  .wrap-overlay > .content.html ol {
    margin-bottom: 20px;
    padding-left: 10px;
  }
  .wrap-overlay > .content.html ol li {
    position: relative;
    margin-bottom: 5px;
    padding-left: 10px;
    line-height: 26px;
    font-size: 14px;
  }
  .wrap-overlay > .content.html ol li p {
    padding-left: 20px;
    margin-bottom: 5px;
  }
  .wrap-overlay > .content.html p {
    line-height: 26px;
    font-size: 14px;
    margin-bottom: 20px;
  }
</style>`;
$('head').append(overlay_css);
// オーバーレイを消す
$('.wrap-overlay > .close-area, .wrap-overlay > .content > .close').on('click',function(){
  $('.wrap-overlay').removeClass('show');
  // $('body').removeClass('noscroll');
  setTimeout(() => {
    $('.wrap-overlay > .content > .inner > .insert').children().remove();
    $('.wrap-overlay > .content').removeClass('html');
  },250);
});
// funk.overlay 画像
var overlayImgShow = function(device, btn) {
  file_name = btn.attr("class").split(' ')[0].replace("overlay-img-", "").slice(0, -7);
  $('.wrap-overlay > .content > .inner > .insert').append('<img src="js/overlay/img/'+device+'/'+file_name+'.png" srcset="js/overlay/img/'+device+'/'+file_name+'.png 1x, js/overlay/img/'+device+'/'+file_name+'@2x.png 2x">');
  $('.wrap-overlay').addClass('show');
  // $('body').addClass('noscroll');
};
// funk.overlay HTML
var overlayHtmlShow = function(btn) {
  file_name = btn.attr("class").split(' ')[0].replace("overlay-html-", "").slice(0, -7);
  $.ajax({
    url : "js/overlay/html/"+file_name+".txt", type : "GET", dataType: 'html',
    success: function(data) {  $('.wrap-overlay > .content > .inner > .insert').append(data);},
    error:function() { console.log("readError:"+file_name); }
  });
  $('.wrap-overlay').addClass('show');
  // $('body').addClass('noscroll');
};
$(function() {
  // オーバーレイ 画像を表示する
  $('[class^="pc-wrap"] [class^="overlay-img-"]').on('click',function(){
    overlayImgShow('pc', $(this));
  });
  $('[class^="tablet-wrap"] [class^="overlay-img-"]').on('click',function(){
    overlayImgShow('pc', $(this));
  });
  $('[class^="sp-wrap"] [class^="overlay-img-"]').on('click',function(){
    overlayImgShow('sp', $(this));
  });
  // HTMLを表示するオーバーレイ
  $('[class^="overlay-html-"]').on('click',function(){
    $('.wrap-overlay > .content').addClass('html');
    overlayHtmlShow($(this));
  });
});

// ************************************** //
// **SEO・meta用テキスト読み込み
var read_file;
var target;

// ページタイトル
read_file = "seo/title/"+file_name +  ".txt";
$.ajax({
  url : "js/"+read_file, type : "GET", dataType: 'html',
  success: function(data) { $('head').append('<title>'+data+'</title><meta property="og:title" content="'+data+'">'); },//取得成功したら実行する処理
  error:function() { console.log("readError:"+read_file); } //取得失敗時に実行する処理
});

// ディスクリプション
read_file = "seo/description/"+file_name +  ".txt";
$.ajax({
  url : "js/"+read_file, type : "GET", dataType: 'html',
  success: function(data) { $('head').append('<meta name="description"  content="'+data+'"><meta property="og:description" content="'+data+'">');},//取得成功したら実行する処理
  error:function() { console.log("readError:"+read_file); } //取得失敗時に実行する処理
});

// キーワード
read_file = "seo/keywords/"+file_name +  ".txt";
$.ajax({
  url : "js/"+read_file, type : "GET", dataType: 'html',
  success: function(data) { $('head').append('<meta name="keywords"  content="'+data+'">');},//取得成功したら実行する処理
  error:function() { console.log("readError:"+read_file); } //取得失敗時に実行する処理
});

// ファビコン
var favicon= (function(param) {return param[0].replace(/\n|\r/g, "");})`
<link rel="icon" type="image/x-icon" href="js/favicon/favicon.svg">
<link rel="apple-touch-icon" href="js/favicon/appletuch_icon.png" sizes="180x180">
<link rel="icon" type="image/png" href="js/favicon/android-touch-icon.png" sizes="192x192">
`;
$('head').append(favicon);

// OGP
read_file = "seo/title/index.txt"; // サイト名
$.ajax({
  url : "js/"+read_file, type : "GET", dataType: 'html',
  success: function(data) { $('head').append('<meta property="og:site_name" content="'+data+'">'); },//取得成功したら実行する処理
  error:function() { console.log("readError:"+read_file); } //取得失敗時に実行する処理
});
var ogp= (function(param) {return param[0].replace(/\n|\r/g, "");})`
<meta property="og:type" content="website" />
<meta property="og:url" content="`+ location.href.replace(/#.*$/,"") +`" />
<meta property="og:image" content="js/ogp.jpg" />
<meta name="twitter:card" content="summary_large_image" />
<meta property="og:locale" content="ja_JP" />
`;
$('head').append(ogp);

// SEO文章
$('body').append('<div id="seo" style="position:absolute;left:0;top:0;z-index:0;overflow:scroll;width:80vw;height:80vh;"></div><div style="position:absolute;z-index:1;background:#fff;width:80vw;height:80vh;left:0;top:0;"></div>');
target = $('#seo');
read_file = "seo/contents/"+file_name +  ".txt";
$.ajax({
  url : "js/"+read_file, type : "GET", dataType: 'html',
  success: function(data) { target.html(data); },//取得成功したら実行する処理
  error:function() { console.log("readError:"+read_file); } //取得失敗時に実行する処理
});

// ************************************** //
// ** SATORI+TIMEREX

/* 導入個所に以下を挿入（●●）は個別の値
<div id="satori__creative_container">
<script id="-_-satori_creative-_-" src="//delivery.satr.jp/js/creative_set.js" data-key="●●"></script>
</div>

<div id="timerex">
<h1>無料相談（オンライン打ち合わせ）の予約はこちら</h1>
<!-- Begin TimeRex Widget -->
<div id="timerex_calendar" data-url="https://timerex.net/s/●●/●●"></div>
<script id="timerex_embed" src="https://timerex.net/js/embed.js"></script>
<script type="text/javascript">TimerexCalendar();</script>
<!-- End TimeRex Widget -->
</div>
*/

if(USE_SATORI == true) {
  var content_satori = 0;

  $(window).on('load', function(){
    // SATORI コンテンツ内容取得
    if(content_satori == 0 && $("#satori__creative_container").find("#satori__custom_form").length !== 0) content_satori = $("#satori__creative_container").children();
  });

  // SATORI送信後にTIMEREX表示
  $(document).on('click', '.satori__submit_group' , function() {
    setTimeout($.proxy(function(){
      if($(this).parent().parent().hasClass('satori__hide')) {
        $('#timerex').addClass('show');
      }
    } ,this),3000);
  });
  
  // CSS
  var satori_css = (function(param) {return param[0].replace(/\n|\r/g, "");})`
  <style>
    [class^="relative-satori"] {
      font-size: 14px;
      background: #F8F8F8 !important;
      padding: 40px; }
      [class^="relative-satori"] .satori__custom_form {
        background: transparent !important; }
      [class^="relative-satori"] .satori__custom_form .satori__notes {
        background: transparent !important;
        border: 0 !important;
        padding: 6px 0; }
      [class^="relative-satori"] .satori__submit_group {
        margin-bottom: 10px !important; }
      [class^="relative-satori"] .agreement-link-wrap {
        margin-bottom: 20px; }
        [class^="relative-satori"] .agreement-link-wrap .link {
          position: relative;
          text-decoration: underline;
          font-size: 14px;
          display: block;
          margin-bottom: 0;
          padding-left: 35px;
          color: #000; }
          [class^="relative-satori"] .agreement-link-wrap .link:hover {
            color: #666; }
          [class^="relative-satori"] .agreement-link-wrap .link + br {
            display: none; }
          [class^="relative-satori"] .agreement-link-wrap .link::before {
            content: " ";
            position: absolute;
            left: 10px;
            bottom: 6px;
            width: 13px;
            height: 13px;
            background: url("../images/icon-arrow-navy.svg") no-repeat center center/100% 100%; }
      [class^="relative-satori"] #timerex {
        opacity: 0;
        max-height: 0px;
        overflow: hidden;
        transition: 1s all ease; }
        [class^="relative-satori"] #timerex.show {
          max-height: 2000px;
          opacity: 1; }
  </style>`;
  $('head').append(satori_css);

  // funk.satori
  // SATORIは、ページ内で1か所しか配置できないため、ＰＣ／タブレット／スマホ切り替え時に、要素を切り取り貼り付けする
  var satori = function(content_satori) {
    setTimeout(() => {
      if($("#satori__creative_container").find("#satori__custom_form").length == 0) {
        $("#satori__creative_container").children().remove();
        $("#satori__creative_container").append(content_satori);
      }
    },1000);
  }
}

// ************************************** //
// ** 各デバイスの処理（ウィンドウサイズ切り替え時のイベント）変数は予め上部で設定
$(window).on('load resize', function(){
  window_width = document.documentElement.clientWidth || document.body.clientWidth;

  // コンテンツ内容取得
  if(content_pc == 0) content_pc = $('[class^="pc-wrap"]').children();
  if(content_tablet == 0) content_tablet = $('[class^="tablet-wrap"]').children();
  if(content_sp == 0) content_sp = $('[class^="sp-wrap"]').children();

  if( !is_pc && PC_BREAKPOINT <= window_width) {
    console.log("*********パソコン表示*********");
    is_pc = true;
    is_tablet = false;
    is_sp = false;

    // detach SEO対策（表示デバイス以外の要素を削除）
    if ($('[class^="pc-wrap"]').children().length == 0) $('[class^="pc-wrap"]').append(content_pc);
    setTimeout(() => {
      if ($('[class^="tablet-wrap"]').children().length !== 0) $('[class^="tablet-wrap"]').children().remove();
      if ($('[class^="sp-wrap"]').children().length !== 0) $('[class^="sp-wrap"]').children().remove();
    },100);
    
    // SATORI挿入
    if(USE_SATORI == true) {
      satori(content_satori);
    }

    loading('pc', content_pc); // ロード画面
    lazyload('pc'); // lazyload


  } else if ( !is_tablet && (TABLET_BREAKPOINT <= window_width && window_width < PC_BREAKPOINT) ) {
    console.log("*********タブレット表示*********");
    is_pc = false;
    is_tablet = true;
    is_sp = false;

    // detach SEO対策（表示デバイス以外の要素を削除）
    if ($('[class^="tablet-wrap"]').children().length == 0) $('[class^="tablet-wrap"]').append(content_tablet);
    setTimeout(() => {
      if ($('[class^="pc-wrap"]').children().length !== 0) $('[class^="pc-wrap"]').children().remove();
      if ($('[class^="sp-wrap"]').children().length !== 0) $('[class^="sp-wrap"]').children().remove();
    },100);
    
    // SATORI挿入
    if(USE_SATORI == true) {
      satori(content_satori);
    }

    loading('tablet', content_tablet); // ロード画面
    lazyload_order('tablet'); // lazyload

    // Animaアニメーション要素表示スクリプト上書き
    //（transformで横幅768pxサイズに変更し、スクロール位置が変わるため、判定位置縮小）
    ShowOnScroll.prototype.onScroll = function () {
      var screenBottom = (window.pageYOffset + window.innerHeight) / 0.691;
      if (this.nextEventY == undefined || this.nextEventY > screenBottom) {
        return;
      }
      this.nextEventY = undefined;
      for (var i = 0; i < this.toShow.length; i++) {
        var e = this.toShow[i];
        var top = this.getTop(e);
        if (top < screenBottom) {
          this.show(e);
          e.classList.add('anim-showed');
          this.toShow.shift();
          i--;
        } else {
          this.nextEventY = top;
          break;
        }
      }
    };


  } else if ( !is_sp && window_width < TABLET_BREAKPOINT) {
    console.log("*********スマホ表示*********");
    is_pc = true;
    is_tablet = false;
    is_sp = true;
    
    // 表示デバイス以外の要素を削除
    if ($('[class^="sp-wrap"]').children().length == 0) $('[class^="sp-wrap"]').append(content_sp);
    setTimeout(() => {
      if ($('[class^="pc-wrap"]').children().length !== 0) $('[class^="pc-wrap"]').children().remove();
      if ($('[class^="tablet-wrap"]').children().length !== 0) $('[class^="tablet-wrap"]').children().remove();
    },100);
    
    // SATORI挿入
    if(USE_SATORI == true) {
      satori(content_satori);
    }

    loading('sp', content_sp); // ロード画面
    lazyload('sp'); // lazyload

    
  }
});

// ************************************** //
// **PC・タブレット・スマホ、デバイス共通のCSS
setTimeout(() => {
  var custom_css = (function(param) {return param[0].replace(/\n|\r/g, "");})`
  <style>
    /* imgの隙間を削除 */
    .screen {
      font-size: 0;
    }
    /* 一度表示した要素は消さないように修正 */
    .anim-showed {
      display: block !important;
    }
    /* アニメーション：繰り返し */
    [class^="infinite"], [class^="lazy infinite"] {
      animation-iteration-count: infinite;animation-duration: 1s;
    }
    /* アニメーション：ワイプテキスト（XD親の要素がレスポンシブデザインになっているとうまく表示されない） */ {
      .screen div[class^="wraptext"]{
        overflow: hidden;
      }
    }
    /* コンテンツ：Anima修正：Width100%表示、screen要素 cursor:pointer;削除 */
    .container-center-horizontal {
      overflow: hidden !important;
      position: relative;
      z-index:2;
    }
    .container-center-horizontal > div {
      overflow: visible !important; cursor: auto;
    }
    /* コンテンツ：fixed-がついている要素はz-index:5をつける */
    [class^="fixed-"] {
      z-index:5;
    }
    /* コンテンツ：Totopリンク絶対配置（右下） */
    [class^="fixed-totoplink"] {
      position: fixed; left: auto; top: auto; right: 20px; bottom: 20px;
    }
    /* コンテンツ：タブレットの場合は要素を縮小 */
    [class$="-tablet screen"] {
      height:calc(`+$('[class^="tablet-wrap"]').height()+`px * 0.691) !important;
      transform: scale(0.691);
      transform-origin: top center;
    }
    /* コンテンツ：relativeコンテンツ */
    [class*="relative"] {
      position: relative !important;
      top: auto !important;
      bottom: auto !important;
    }
    /* コンテンツ：高さ可変コンテンツ */
    [class*="height-auto"],
    [class*="auto-height"] {
      height: auto !important;
    }
  </style>`;
  $('head').append(custom_css);
}, 1);