/*
 *
 * © LEANSYSTEM
 * Contact: nabe@lean-system.co.jp
 *
 * Figma・XDでのAnimaの使用方法やプログラムについてのご質問、ご相談などありましたらお気軽にお問い合わせください。
 * 
 */


// func.Js読み込み用関数
var loadScript = function(src, callback = "") {
  var head = document.getElementsByTagName('head')[0];
  var script = document.createElement('script');
  script.src = src;
  head.appendChild(script);

  if(callback != "") {
    callback();
  }
}

// jquery　perfect-scrollbar
setTimeout(() => { // すぐに使わないため、遅延読み込み
  loadScript('//cdnjs.cloudflare.com/ajax/libs/jquery.perfect-scrollbar/1.5.2/perfect-scrollbar.min.js');
  $('head').append('<link href="//cdnjs.cloudflare.com/ajax/libs/jquery.perfect-scrollbar/1.5.2/css/perfect-scrollbar.min.css" rel="stylesheet">');

  // Overlayコンテンツ用にperfect scrollbarを適用
  if( $('.wrap-overlay > .content > .inner').length != 0 ) {
    setTimeout(() => {
      var ps = new PerfectScrollbar('.wrap-overlay > .content > .inner');
    },500);
  }
},3000);

// jquery　easing
loadScript('//cdnjs.cloudflare.com/ajax/libs/jquery-easing/1.4.1/jquery.easing.min.js');



// ************************************** //
// **スムーズスクロール
// 位置・時間調整
var adjust = -100;
var time = 1000;

$(document).on("click", "a", function (event) {
  var hash = this.hash;
  var a = $(hash);
  var c = a.offset();
  var position = c.top + adjust;

  // もし#tのハッシュで、トップページではなかったら、トップページへリダイレクト
  if(hash == "#t" && file_name != "/index") {
    location.href = urlOrigin;
    return false;
  }

  if (c && hash !== "" && this.host == location.host) {
    event.preventDefault();
    $("html, body").animate({ scrollTop: position }, time, 'easeOutExpo', function () {
      if((hash == "#t" || hash == "#mv") && file_name == "/index") {
        window.location.hash = "";
      }
    });
  }
});

// 関数：スムーススクロール
// 指定したアンカー(#ID)へアニメーションでスクロール
function scrollToAnker(hash) {
  var target = $(hash);
  if(target.length != 0) {
    console.log(target);
    var position = target.offset().top + adjust;
    $('body,html').stop().animate({scrollTop:position}, time, 'easeOutExpo');
  }
}
// ページ上部へ戻るための要素を追加
$('body').prepend('<div id="t"></div>');


let nextIdx = 0; // setSrc()が次に処理するimgのインデックス

// classList の中に文字列 "animate-enter" を含むクラスがあるか？
const isAnimateEnter = (elem) => {
  for (let klass of elem.classList) {
    if (~klass.indexOf("animate-enter")) return true;
  }

  return false;
};

// imgのsrcを設定する
const setSrc = (imgTags, index) => {
  $(imgTags[index])
    .attr("src", $(imgTags[index]).attr("data-original"))
    .removeAttr("data-original")
    .on('load', (e) => {
      // classList の中に文字列 "animate-enter" を含むクラスが無い場合、クラス "show" を追加する。
      if (!isAnimateEnter(e.target)) e.target.classList.add("show");

      if (index < imgTags.length) setSrc(imgTags, nextIdx);
    });

  nextIdx++;
};

// ************************************** //
// **delayload
// func.imgDelayLoad
const imgDelayLoad = () => {
  const imgTags = $('.screen').find('img[data-original]'); // data-originalタグの付いている全imgタグを取得
  for (let i = 0; i < 7; i++) setSrc(imgTags, i);
};


// ************************************** //
// **ロード画面
var loading = $('#loading');
// func.ロード画面
var loading = function() {
  $('[class*="firstview"] [class*="anim-"]').css('display', 'none'); // firstviewのアニメーション要素非表示
  $('#loading .progress').height($('.loading-img.bg').height()).width($('.loading-img.bg').width());
  var speed = LOADINGSPEED;

  $('#loading .progress div').height(0);
  $('#loading,#loading .progress div').css({'transition':'none'});
  setTimeout(() => {$('#loading,#loading .progress div').css({'transition':''});},1);
  var loadCount = 0, //loading状況の初期化
  imgtag = $('.screen').find('[class*="firstview"]').find('img[data-original]'); // firstviewの全imgタグを取得（もし無かったら、1秒後にload画面を消す分岐が必要）
  imgtagLength = imgtag.length; //読み込む画像の数を取得

  if(imgtagLength == 0) {
    setTimeout(() => {
      $("#loading .progress div").css({"height": "100%"}); // ローディングバーを100%に
      setTimeout(() => {
        $("#loading").addClass('hide');
        setTimeout(() => {
          $('[class*="firstview"] [class*="anim-"]').css('display', '').addClass("anim-showed"); // firstviewのアニメーション表示
        }, 0);
        window.scroll({top: 0}); // スクロールを0位置にする
        imgDelayLoad(); // 画像遅延読み込み
        if(urlHash) {
          setTimeout(function () {
            scrollToAnker(urlHash) ; // ローディング完了後、時間差でハッシュ位置へスクロール実行
          }, 100);
        }
      }, speed);
    },speed);
  } else {
    imgtag.each(function() {  //  各画像ごとの処理
      var src = $(this).attr("data-original"); // data-original属性の値を取得
      $(this)
        .attr("src", src)
        .removeAttr('data-original')
        .on('load', function(){
          loadCount++; //画像が読み込まれたら、loading状況を更新
        });
    });

    setTimeout(() => {
      var interval = 100;
      var timer = setInterval(function() { //intervalミリ秒間隔でloading状況をローディングバーに反映
        $("#loading .progress div").css({
            "height": (loadCount / imgtagLength) * 100 + "%" //読み込まれた画像の数を画像全体で割り、%としてローディングバーのheightに設定
        });
        loadCount += imgtagLength / 300;
        if((loadCount / imgtagLength) * 100 >= 100){ //100%読み込まれたらローディングバーを隠す
          clearInterval(timer);
            $("#loading .progress div").css({"height": "100%"}); // ローディングバーを100%に
            setTimeout(() => {
              $("#loading").addClass('hide');
              $('[class*="firstview"] [class*="anim-"]').css('display', 'none'); // firstviewのアニメーション要素非表示
              setTimeout(() => {
                $('[class*="firstview"] [class*="anim-"]').css('display', '').addClass("anim-showed"); // firstviewのアニメーション表示
              }, 1000);
              window.scroll({top: 0}); // スクロールを0位置にする
              imgDelayLoad(); // 画像遅延読み込み
              if(urlHash) {
                setTimeout(function () {
                  scrollToAnker(urlHash) ; // ローディング完了後、時間差でハッシュ位置へスクロール実行
                }, 100);
              }
            },speed);
        }
      }, interval);
    }, speed);
  }
};

loading();


// ************************************** //
// ** SLICK スライド要素
// funk.Slick
var slick = function() {
  $('[class*="slick"] [class*="show"]').each(function() {
    $(this).find('[class*="slide"]').removeClass(function(index, className) {
      return (className.match(/\bslide-\S+/g) || []).join(' ');
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

    $(this).slick({
      prevArrow: '<span class="slick-arrows prev"></span>',
      nextArrow: '<span class="slick-arrows next"></span>',
      dots: true,
      infinite: true,
      cssEase: 'ease',
      speed: 1000,
      slidesToShow: show,
      adaptiveHeight: true,
      autoplay: true,
      autoplaySpeed: 5000,
      fade: true
    });
  });
};

if(ISUSE_SLICK) slick();


// ************************************** //
// ** 各デバイスの処理（ウィンドウサイズ切り替え時のイベント）変数は予めvariable.phpで設定

var window_width = window.innerWidth || document.documentElement.clientWidth || document.body.clientWidth;
var is_pc = false; // PC表示識別
var is_tablet = false; // タブレット表示識別
var is_sp = false; // スマホ表示識別
var is_smallsp = false; // 375px未満のスマホ表示識別
if($('[class*="pc-wrap"]').length !== 0) $('[class^="pc-wrap"]').parent(".screen").addClass("content-pc");
if($('[class*="sp-wrap"]').length !== 0) $('[class^="sp-wrap"]').parent(".screen").addClass("content-sp");


var applyTabletDevice = function() {
  // bodyの高さと、pc-wrap要素の高さ*RATIO_SP_TABLET倍が不一致だった場合、調整
  timer = setInterval(function(){
    if( is_tablet === true && $('body').height() != $('[class^="pc-wrap"]').height() * RATIO_SP_TABLET ) {
      $('.content-pc').attr('style', 'height:'+ $('[class^="pc-wrap"]').height() * RATIO_SP_TABLET +'px !important;');
      clearInterval(timer);
    }
  },1);
}

var removeTabletDevice = function() {
  // bodyの高さと、pc-wrap要素の高さが不一致だった場合、調整
  timer = setInterval(function(){
    if( is_tablet === false && $('body').height() != $('[class^="pc-wrap"]').height() ) {
      $('.content-pc').attr('style', '');
      clearInterval(timer);
    }
  },1);
}

var applySmallSpDevice = function() {
  // bodyの高さと、sp-wrap要素の高さ*RATIO_MINSP_SMALLSP倍が不一致だった場合、調整
  timer = setInterval(function(){
    if( is_smallsp === true && $('body').height() != $('[class^="sp-wrap"]').height() * RATIO_MINSP_SMALLSP ) {
      $('.content-sp').attr('style', 'height:'+ $('[class^="sp-wrap"]').height() * RATIO_MINSP_SMALLSP +'px !important;');
      clearInterval(timer);
    }
  },1);
}

var removeSmallSpDevice = function() {
  // bodyの高さと、sp-wrap要素の高さが不一致だった場合、調整
  timer = setInterval(function(){
    if( is_smallsp === false && $('body').height() != $('[class^="sp-wrap"]').height() ) {
      $('.content-sp').attr('style', '');
      clearInterval(timer);
    }
  },1);
}

var applyDefaultAnimaShowOnScroll = function() {
  // Animaアニメーション表示スクリプト上書き
  ShowOnScroll.prototype.onScroll = function () {
    var screenBottom = window.pageYOffset + window.innerHeight;
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
}
var applyTabletAnimaShowOnScroll = function() {
  // Animaアニメーション表示スクリプト上書き
  //（タブレットはRATIO_SP_TABLET倍縮小し、スクロール位置が変わるため判定位置もRATIO_SP_TABLET倍）
  ShowOnScroll.prototype.onScroll = function () {
    var screenBottom = ( window.pageYOffset + window.innerHeight ) / RATIO_SP_TABLET;
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
}
var applySmallSpAnimaShowOnScroll = function() {
  // Animaアニメーション表示スクリプト上書き
  //（375px以下のデバイスはRATIO_MINSP_SMALLSP倍縮小し、スクロール位置が変わるため判定位置もRATIO_MINSP_SMALLSP倍）
  ShowOnScroll.prototype.onScroll = function () {
    var screenBottom = ( window.pageYOffset + window.innerHeight ) / RATIO_MINSP_SMALLSP;
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
}

$(window).on('load resize', function(){
  window_width = window.innerWidth || document.documentElement.clientWidth || document.body.clientWidth;

  if($('[class*="pc-wrap"]').length !== 0) { // pc-wrap要素がある場合
    if( is_pc == false && TABLET_BREAKPOINT <= window_width) {
      console.log("*********パソコン表示開始*********");
      is_pc = true;
      is_tablet = false;

      removeTabletDevice();
      applyDefaultAnimaShowOnScroll();
    }

    if( is_tablet == false && window_width < TABLET_BREAKPOINT) {
      console.log("*********タブレット表示開始*********");
      is_pc = false;
      is_tablet = true;

      applyTabletDevice();
      applyTabletAnimaShowOnScroll();
    }
  }

  if($('[class*="sp-wrap"]').length !== 0) { // sp-wrap要素がある場合
    if( is_sp == false && SMALLSP_BREAKPOINT <= window_width ) {
      console.log("*********スマホ表示開始*********");
      is_sp = true;
      is_smallsp = false;

      removeSmallSpDevice();
      applyDefaultAnimaShowOnScroll();
    }

    if( is_smallsp == false && window_width < SMALLSP_BREAKPOINT ) {
      console.log("*********375px未満スマホ表示開始*********");
      is_sp = false;
      is_smallsp = true;

      applySmallSpDevice();
      applySmallSpAnimaShowOnScroll();
    }
  }

});



// ************************************** //
// ** スクロール時のアクション

$(window).on('scroll', function(){
  var scrollposition = $(this).scrollTop();

  // ナビゲーション
  if(scrollposition > 100) {
    $('[class^="fixed-header"]').addClass('scroll');
  } else {
    $('[class^="fixed-header"]').removeClass('scroll');
  }

  // トップへ戻る
  if(scrollposition > 600) {
    $('[class^="fixed-totop"]').css({'opacity':1});
  } else {
    $('[class^="fixed-totop"]').css({'opacity':0});
  }
});



// ************************************** //
// ** クリック時のアクション
$('body').on('click', '#designcheck-btn', function() {
  $('#designcheck').fadeToggle();
});

$('body').on('click', '[class^="fixed-pagetop"], [class^="fixed-totop"], [class^="pagetop"]',function(){
  $('body,html').stop().animate({scrollTop:0}, 1000, 'easeOutExpo');
});

$('body').on('click', '[class*="spmenu-open-btn"]',function(){
  $('[class^="sp-wrap"]').parent().find('[class*="fixed-spmenu"]').addClass('show');
});

$('body').on('click', '[class*="spmenu-close-btn"]',function(){
  $('[class^="sp-wrap"]').parent().find('[class*="fixed-spmenu"]').removeClass('show');
});

$('body').on('click', '[class*="fixed-spmenu"] [class*="link-"]',function(){
  $('[class*="fixed-spmenu"]').removeClass('show');
});


// ************************************** //
// ** AnimaScript
function ShowOnScroll() {
  this.toShow = [];
  this.nextEventY = undefined;
}

ShowOnScroll.prototype.show = function (e) {
  e.style.display = "";
};

ShowOnScroll.prototype.hide = function (e) {
  e.style.display = "none";
};

ShowOnScroll.prototype.getTop = function (e) {
  if (e.Top != undefined && e.Top != 0) {
    return e.Top;
  }
  var top = 0;
  var iter = e;
  do {
    top += iter.offsetTop || 0;
    iter = iter.offsetParent;
  } while (iter);
  e.Top = top;
  return top;
};

ShowOnScroll.prototype.onScroll = function () {
  var screenBottom = window.pageYOffset + window.innerHeight;
  if (this.nextEventY == undefined || this.nextEventY > screenBottom) {
    return;
  }
  this.nextEventY = undefined;
  for (var i = 0; i < this.toShow.length; i++) {
    var e = this.toShow[i];
    var top = this.getTop(e);
    if (top < screenBottom) {
      this.show(e);
      e.classList.add('anim-showed'); // 2023年1月19日追加
      this.toShow.shift();
      i--;
    } else {
      this.nextEventY = top;
      break;
    }
  }
};

ShowOnScroll.prototype.resetScrolling = function () {
  // Clear state
  var screenBottom = window.pageYOffset + window.innerHeight;
  for (var i = 0; i < this.toShow.length; i++) {
    var e = this.toShow[i];
    this.show(e);
  }
  this.toShow = [];
  this.nextEventY == undefined;

  // Collect items
  var itemsToShowOnScroll = Array.prototype.slice.call(document.getElementsByTagName("*"));
  itemsToShowOnScroll = itemsToShowOnScroll.filter(function (e) {
    // return e.getAttribute("show-on-scroll") != undefined; // 2023年1月19日ここを変更
    return String(e.getAttribute("class")).indexOf('anim-') !== -1 || e.getAttribute("show-on-scroll") != undefined;
  });
  var getTop = this.getTop;
  itemsToShowOnScroll.sort(function (a, b) {
    return getTop(a) - getTop(b);
  });
  for (var i = 0; i < itemsToShowOnScroll.length; i++) {
    var e = itemsToShowOnScroll[i];
    var top = this.getTop(e);
    this.toShow.push(e);
    this.hide(e);
    this.nextEventY = this.nextEventY != undefined ? this.nextEventY : top;
  }
};

ShowOnScroll.prototype.handleEvent = function (e) {
  switch (e.type) {
    case "scroll":
      this.onScroll();
      break;
    case "resize":
      this.resetScrolling();
      break;
  }
};

ShowOnScroll.prototype.init = function () {
  this.resetScrolling();
  window.addEventListener("scroll", this);
  window.addEventListener("resize", this);
};

// After anima-src
setTimeout(function () {
  var instShowOnScroll = new ShowOnScroll();
  instShowOnScroll.init();
}, 250);


// ************************************** //
// ** jsが読み込まれたら、bodyのopacityを取る
document.getElementsByTagName('body')[0].style.opacity = '1';
