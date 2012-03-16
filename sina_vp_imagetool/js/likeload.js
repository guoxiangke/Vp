$().ready(function(){
	$("#edit-submit").click(function(){
		var word=$(this).parent().find('textarea').val();
		var userPicsrc=$('.user-img img').attr('src');
		var userName=$('.user-name p').text();
		//alert(userPicsrc);
		words="<div class='logi_box'><a id='comment-272'></a><div id='272' class='clear' rel='272'><div  class='fbcom_nr'><div class='wb_nr  mjvp-content-child-news-nr'><div class='userPic mjcontent-child-news-nr-img'><img width='38'  height='38'src='"+userPicsrc+"'></div><div class='msgBox402 mjcontent-child-news-nr-body'><!--div class='fbcom_yh'></div--><div  class='msgCnt mjcontent-child-news-nr-title'><a href='/UCenter/0/51' style='color:#0078b6;'>"+userName+"</a>说：<p>"+word+"</p><span class='new'></span></div><div  class='pubInfo'><span class='copy_time'>刚刚</span><span class='right'><div class='links'><ul class='links mjcontent-child-news-nr-tm'><li class='comment_reply  first last mjhhf'><a href='/comment/reply/275/272'>回复</a></li></ul></div></span></div></div></div></div></div>";
		var flag=$('.box');
		var target_url=$(this).parents().find('form').attr('action');
		var target_form=$(this).parents().find('form');
		var location_url="http://dev.weipujie.com";
		//alert(target_form.serialize());
	/**
	*	
		$.ajax({
		   type: "POST",
		   url: location_url+target_url,
		   data: target_form.serialize(),
		   success: function(msg){
		     flag.after(words);
		   }
		});
	*/
		//修正评论
		var allComments=document.getElementsByClassName('logi_box');
		
		if(allComments.length>5){							
			$('.logi_box:last').remove();
		}
		
	})
})