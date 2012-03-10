$().ready(function(){
	$('.box form').submit(function(){
			return false;

	})
	$("#edit-submit").click(function(){
		var word=$(this).parent().find('textarea').val();
		var userPicsrc=$('.user-img img').attr('src');
		var userName=$('.user-name p').text();
		//alert(userPicsrc);
		words="<div class='logi_box'><a id='comment-272'></a><div id='272' class='clear' rel='272'><div class='fbcom_nr'><div class='wb_nr  content-child-news-nr-body'><div class='userPic content-child-news-nr-img'><img width='38' height='38'src='"+userPicsrc+"'></div><div class='msgBox402 vp-content-child-news-nr'><!--div class='fbcom_yh'></div--><div class='msgCnt'><a href='/UCenter/0/51'>"+userName+"</a>说：<p>"+word+"</p><span class='new'></span></div><div class='pubInfo'><span class='copy_time'>刚刚</span><span class='right'><div class='links'><ul class='links'><li class='comment_reply first last'><a href='/comment/reply/275/272'>回复</a></li></ul></div></span></div></div></div></div></div>";
		var flag=$('.box');
		flag.after(words);
		var allComments=document.getElementsByClassName('logi_box');
		
		if(allComments.length>5){							
			$('.logi_box:last').remove();
		}
		var target_url=$(this).parents().find('form').attr('action');
		var location_url="http://dev.weipujie.com/";
		$.ajax({
		   type: "POST",
		   url: location_url+target_url,
		   data: "comment=word",
		   success: function(msg){
		     //alert( 'DONE' );
		   }
		});
	})
})