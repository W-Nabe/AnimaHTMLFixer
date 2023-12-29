<?php

// Instagramの投稿データを取得するPHPのクラスを作成
class IGMediaDataFetcher {
  private $access_token;
  private $instagram_business_account_id;
  private $limit;

  public function __construct($instagram_business_account_id, $access_token, $limit = 10) {
    $this->access_token = $access_token;
    $this->user_id = $instagram_business_account_id;
    $this->limit = $limit;
  }

  public function getMediaData() {

    $endpoint = "https://graph.facebook.com/v16.0/{$this->user_id}/?fields=name,media.limit({$this->limit}){caption,media_url,thumbnail_url,permalink,username,timestamp}&access_token={$this->access_token}";

    // Use file_get_contents() to fetch the data
    $response = file_get_contents($endpoint);

    if ($response === false) {
      throw new Exception('Failed to fetch data using file_get_contents()');
    }

    $data = json_decode($response, true);

    // Check if the API call was successful
    if (isset($data['media']['data'])) {
      return $data['media']['data'];

    } else {
      throw new Exception('Failed to fetch media data from Instagram API.');
    }
  }
}

$instagram_business_account_id = "17841464027121732"; 
$access_token = "EAADjuyZCdA9gBO1gwcLuYxe0CZCxlfw7YkbRX2ZAzwPUhWdZAgL9vrb0Da58NSDiM8WmJ1o7qZBchMs8iZAbZB0CdM7KBFk0vrZCniztokS0klzrlBV4Y2ZAoZC0nfL6a1e7CxuSe5oE1tPvdP1wbv1ub6GTZA9MpQukzgZADXpHyx9eNGoJbYQokZCtLQlBpwrxEFdwd"; 
$limit = 3; // 取得件数（任意）

// 参考：インスタビジネスアカウントIDとアクセストークン取得方法
// https://quattroinc.jp/blog/52/
// https://teratail.com/questions/210018

try {
    $igMediaDataFetcher = new IGMediaDataFetcher($instagram_business_account_id, $access_token, $limit);
    $igPosts = $igMediaDataFetcher->getMediaData();
} catch (Exception $e) {
    echo 'Error: ' . $e->getMessage();
}

if( $igPosts ): ?>
<div class="container">
  <div class="flex flex-wrap flex-nowrap justify-between md:gap-4">
  <?php foreach ( $igPosts as $post ): ?>
    <div class="md:w-1/3">
      <div class="h-full border-2 rounded-lg overflow-hidden bg-color">
        <img class="lg:h-48 md:h-36 w-full object-cover object-center" src="<?php echo $post['thumbnail_url'] ?? $post['media_url']; ?>" alt="">
        <div class="text">
          <p class="mb-3"><?php echo $post['caption'] ?></p>
          <p class="mb-3"><?php echo date('Y年m月d日', strtotime($post['timestamp']) + 9 * 3600); // Y-m-d H:i ?></p>
          <div class="flex items-center flex-wrap ">
            <a class="text-indigo-500 items-center md:mb-2 lg:mb-0" href="<?php echo $post['permalink']; ?>">Instagramへ
            </a>
          </div>
        </div>
      </div>
    </div>
  <?php endforeach; ?>
  </div>
</div>
<?php endif; ?>

<link href="https://unpkg.com/tailwindcss@^2/dist/tailwind.min.css" rel="stylesheet">
<style>
  #include3 .bg {
    height: 100% !important;
  }
  #instagram {
    font-size: 12px;
  }
  #instagram .flex.anim-showed {
    display: flex !important;
  }
  #instagram .text {
    padding: 10px;
  }
  #instagram .bg-color {
    background: #eafaff;
  }
  #instagram a:hover {
    text-decoration: underline;
  }
</style>