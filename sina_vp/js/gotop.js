// $Id: sina_open.js,v 1.2 2011/02/11 03:52:44 eastcn Exp $
Drupal.behaviors.gotop = function (context) {
	//点评 分享 ajax
	 $(".vp_comment_add").click(function(){
	 			nid=$(this).attr('nid');
	 			var c = $('#Wrap_'+nid);
	 			var  fc= $('#Wrap_comment_'+nid); //额外的
		 		if(!c.attr('style')||c.attr('style')=='display: none;'){
		 			c.slideDown(1000);
		 		}
		 		if(c.attr('style')=='display: block;'&&c.hasClass('vp_comment')){
		 			c.slideUp(1000);
		 		}
		 		if (!fc.hasClass('posted')){
		 			c.html('<img src='+c.attr("loading")+'>');
		 			$.ajax({
							url:  $(this).attr("request"),
							type: 'POST',
							data: '',
							dataType: 'json',
						  success: function(json) {							
						  	fc.html(json.data).addClass('posted');					
						  	c.html(fc.html()).addClass('vp_comment').removeClass('vp_share');
						  	var form = c.find('form');
						 		form_ajax_submit(form,c);
						  },
						  error: function(XMLHttpRequest, textStatus, errorThrown){
							alert('发生错误，请联系管理员');
							}
						});
				}else{
					c.html(fc.html()).addClass('vp_comment').removeClass('vp_share');
					var form = c.find('form')
				  form_ajax_submit(form,c);
				}
	 });
	 function form_ajax_submit(form,c){
	 	 	form.submit(function(e){
	 	 		c.append('<div><img src='+c.attr("loading")+'></div>');
	  		e.preventDefault();
	  		console.log('preventDefault',e);		
				$.ajax({
					url: form.attr("action"),
					type: 'POST',
					data: form.serialize(),
					//dataType: 'json',
				  success: function(json) {
				  	alert('操作成功!');
				  	c.slideUp(1000);
				  },
				  error: function(XMLHttpRequest, textStatus, errorThrown){
				  	console.log('XMLHttpRequest',XMLHttpRequest);
				  	console.log('textStatus',textStatus);
				  	console.log('errorThrown',errorThrown);	
					}
				});		
			});
	 } 
	 $(".vp_share_add").click(function(){
	 			nid=$(this).attr('nid');
	 			tid=$(this).attr('tid');
	 			var c = $('#Wrap_'+nid);
	 			var  fc= $('#Wrap_share_'+nid); //额外的
	 			if(!c.attr('style')||c.attr('style')=='display: none;'){
		 			c.slideDown(1000);
		 		}
		 		if(c.attr('style')=='display: block;'&&c.hasClass('vp_share')){
		 			c.slideUp(1000);
		 		}
		 		//forward/$taxonomy_id/$node->nid/ajax
		 		if (!fc.hasClass('posted')){
		 			c.html('<img src='+c.attr("loading")+'>');
		 			$.ajax({
							url:  $(this).attr("request"),
							type: 'POST',
							data: '',
							dataType: 'json',
						  success: function(json) {
						  	fc.html(json.data).addClass('posted');					
						  	c.html(fc.html()).addClass('vp_share').removeClass('vp_comment');
						  	var form = c.find('form');
						 		form_ajax_submit(form,c);
						  },
						  error: function(XMLHttpRequest, textStatus, errorThrown){
							alert('发生错误，请联系管理员');
							}
						});
				}else{
					c.html(fc.html()).addClass('vp_share').removeClass('vp_comment');
					var form = c.find('form')
					form_ajax_submit(form,c);
				}
		 	}); 	

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
