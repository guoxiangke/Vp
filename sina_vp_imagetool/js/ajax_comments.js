Drupal.behaviors.ajax_comments = function (context) {
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
						  	var form = c.find('form')//#comment-form-1 
						  	form.submit(function(e){
						  		e.preventDefault();//阻止默认提交
						  		if(form.next('textarea#edit-comment-1'=='')){						  			  
						  			// alert("评论内容不能为空！");
						  		}
						  		{//ajax 提交
										$.ajax({
											url: form.attr("action"),
											type: 'POST',
											data: form.serialize(),
											dataType: 'json',
										  success: function(json) {
										  	console.log(json,1);				
										  	//c.html(json);//判断评论成功，提示后，隐藏评论框！！！！
										  },
										  error: function(XMLHttpRequest, textStatus, errorThrown){
										  	console.log(XMLHttpRequest);
										  	console.log(textStatus);
										  	console.log(errorThrown);						
												alert('发生错误，1211111');
											}
										});
						  		}	   
								   
								  });
						  },
						  error: function(XMLHttpRequest, textStatus, errorThrown){
							alert('发生错误，请联系管理员');
							}
						});
				}else{
					c.html(fc.html()).addClass('vp_comment').removeClass('vp_share');
						c.find('#comment-form-1').submit(function(e){
								    e.preventDefault();
								    alert("Submit prevented");
								  });
						  	console.log(c.find('#edit-submit-1'));
				}
			
		 	});
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
						  },
						  error: function(XMLHttpRequest, textStatus, errorThrown){
							alert('发生错误，请联系管理员');
							}
						});	
						//c.slideDown(1000);
				}else{
					c.html(fc.html()).addClass('vp_share').removeClass('vp_comment');
				}
				
		 	});
}
