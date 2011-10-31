<?php

session_start();
include_once( 'config.php' );
include_once( 'weibooauth.php' );


$c = new WeiboClient( WB_AKEY , WB_SKEY , $_SESSION['last_key']['oauth_token'] , $_SESSION['last_key']['oauth_token_secret']  );
$ms  = $c->home_timeline(); // done
$me = $c->verify_credentials();


?>	
<html>
<head>
<title>微博测试应用—luchanghong.com</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<style type="text/css">
		body{ 
			background:url('bg.jpg') fixed no-repeat;
			background-position:center 28px top 0;
			background-color:#4393CF;
			color:gray;
			font-size:13px;
		}
		#head{
			background:url('head.jpg') repeat-x;
			margin:0 auto;
			width:760px;
			padding:0 10px;
			height:160px;
		}
		#head img{
			display:block;
			float:left;
			margin-top:35px;
			margin-left:30px;
		}
		#send_weibo{
			display:block;
			float:left;
			height:	80px;
			width:480px;
			border:#E9F2FB 2px solid;
			margin-top:35px;
			margin-left:20px;
			vertical-align:top;
		}
		#send{
			position:relative;
			bottom:-95px;
		}
		#list{
			/*background:#D5E5F3;*/	
			width:760px;
			margin:10px auto;
			margin-top:0;
			padding:15px 10px;
		}
		#list .list_text{
			color:#02537C;
		}
		#list .image{
			float:left;
			margin-right:20px;
		}
		#list .friends{
			float:left;
			width:480px;
		}
		#list .other_info{
			float:right;
			width:190px;
			font-size:12px;
			color:blue;
		}
		.other_info a{
			text-decoration:none;
		}
		h2{
			color:white;
			font-size:16px;
		}
		#hello{
			position:fixed;
			left:20px;
			top:50px;
			width:130px;
			height:90px;
			padding:10px;
			background:#ABBBCB;
		}

</style>
</head>
<body>	

<h2 id="hello"><?=$me['name'];?>，你好<br>欢迎来到我的微博开发测试页！
	<script type="text/javascript" charset="utf-8">
			(function(){
			  var _w = 106 , _h = 24;
			  var param = {
				url:location.href,
				type:'5',
				count:'', /**是否显示分享数，1显示(可选)*/
				appkey:'3004270161', /**您申请的应用appkey,显示分享来源(可选)*/
				title:'', /**分享的文字内容(可选，默认为所在页面的title)*/
				pic:'', /**分享图片的路径(可选)*/
				ralateUid:'1874021161', /**关联用户的UID，分享微博会@该用户(可选)*/
				rnd:new Date().valueOf()
			  }
			  var temp = [];
			  for( var p in param ){
				temp.push(p + '=' + encodeURIComponent( param[p] || '' ) )
			  }
			  document.write('<iframe allowTransparency="true" frameborder="0" scrolling="no" src="http://hits.sinajs.cn/A1/weiboshare.html?' + temp.join('&') + '" width="'+ _w+'" height="'+_h+'"></iframe>')
			})()
		</script>

</h2>
<div id="head">

	<form action="weibolist.php" >
		<img src="<?=$me['profile_image_url'];?>" width="80" height="80">
		<textarea name="text" id="send_weibo" rows="" cols="" /></textarea>
		&nbsp;<input type="submit" value="发表" id="send" />

	</form>
	
	<!-- <form action="weibolist.php" >
	<input type="text" name="avatar" style="width:300px" value="头像url" />
	&nbsp;<input type="submit" value="更新" />
	</form> 
	

	<h2>发送图片微博</h2>
	<form action="weibolist.php" >
	<input type="text" name="text" style="width:300px" value="文字内容" />
	<input type="text" name="pic" style="width:300px" value="图片url" />
	&nbsp;<input type="submit" value="发表" />
	</form>	  -->
		 
</div>

<div style="height:20px;clear:both;"></div>

<?php

if( isset($_REQUEST['text']) || isset($_REQUEST['avatar']) )
{

if( isset($_REQUEST['pic']) )
	$rr = $c ->upload( $_REQUEST['text'] , $_REQUEST['pic'] );
elseif( isset($_REQUEST['avatar']  ) )
	$rr = $c->update_avatar( $_REQUEST['avatar'] );
else
	$rr = $c->update( $_REQUEST['text'] );	


}

?>

<div id="list">
	<?php if( is_array( $ms ) ): ?>
	<?php foreach( $ms as $item ): ?>

	<div class="list_text">
		<div class="image"><img src="<?=$item['user']['profile_image_url'];?>"></div>
		<div class="friends"><span style="color:#0068A0;"><?=$item['user']['name'];?>：</span><?=$item['text'];?></div>	 
		<div class="other_info"><?=$item['user']['location'];?> | <?=$item['source'];?></div>
		<div style="margin-bottom:10px;height:10px;border-bottom:#ccc dashed 1px;clear:both;"> </div>
	</div>

	<?php endforeach; ?>
	<?php endif; ?>
</div>

</body>
</html>