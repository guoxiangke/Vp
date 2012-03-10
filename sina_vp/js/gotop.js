// $Id: sina_open.js,v 1.2 2011/02/11 03:52:44 eastcn Exp $
Drupal.behaviors.gotop = function (context) {
	$('.top').attr("id", "top");
	//首先将#back-to-top隐藏

	$("#go-top").hide();

	//当滚动条的位置处于距顶部100像素以下时，跳转链接出现，否则消失

	$(function() {
		$(window).scroll(function() {
			if($(window).scrollTop() > 100) {
				$("#go-top").fadeIn(500);
			} else {
				$("#go-top").fadeOut(500);
			}
		});
		//当点击跳转链接后，回到页面顶部位置

		$("#go-top").click(function() {console.log(12398);
			$('body,html').animate({
				scrollTop : 0
			}, "fast", 'linear');
			return false;
		});
	});

  //真人秀
  					 $('.zrx-li').mouseover(function() {  					 	
					 		var which = $(this).attr('id');
					 		//$(this).parent().css('background-color', 'red');//.addClass("current_zrx-li");
					 		$('.current_zrx').toggle().removeClass('current_zrx');
					 		$('.'+which).toggle().addClass("current_zrx");
						});	
	//分享 评论 $('#Wrap_'+nid).toggle(1000);
		 	$('.hidden_next_input').click(function(){
		 		$(this).next(".ajax").removeClass("hidden");		 		
		 	
		 	});
	///?q=user/47/edit 请在这里输入您的邮箱  
	$('#user-profile-form #edit-mail').click(function(){
		 		if($(this).attr("value")=='请在这里输入您的邮箱'){
		 			$(this).attr("value",'');
		 		}
		 	}).blur(function(){
		 		if($(this).attr("value")==''){
		 			$(this).attr("value",'请在这里输入您的邮箱');
		 		}
		 	});
}
