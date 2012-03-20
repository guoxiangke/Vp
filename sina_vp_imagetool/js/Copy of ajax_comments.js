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
						  	setTimeout(form.submit(function(e){
						  		e.preventDefault();//阻止默认提交
						  		if(form.find('#edit-comment').val()==''){						  			  
						  			// alert("评论内容不能为空！");
						  			return false;
						  		}
						  		{//ajax 提交
						  				var my_box=form.parent().parent();
						  				//$(this).attr('disabled','disabled');
										$.ajax({
											url: form.attr("action"),
											type: 'POST',
											data: form.serialize(),
											//dataType: 'html',
										  success: function(json) {
										  	//console.log(json,1);				
										  	//c.html(json);//判断评论成功，提示后，隐藏评论框！！！！
										  //	console.log(form.find('#edit-comment').val());
										  	//处理时间
										  	    	var  d = new Date();
											//星期一, 2012-03-19 17:49
													var day=d.getDay();
													switch (day){
														case 0: day='星期天';
														break;
														case 1: day='星期一';
														break;
														case 2: day='星期二';
														break;
														case 3: day='星期三';
														break;
														case 4: day='星期四';
														break;
														case 5: day='星期五';
														break;
														case 6: day='星期天';
														break;
													}
													var year=d.getFullYear();
													var month=d.getMonth();
													if(month<10){"0"+String(month);}
													var date=d.getDate();
													if(date<10){"0"+String(date);}
													var hours=d.getHours()+1;
													var minutes=d.getMinutes()+1;
											    	var out_time=day+","+year+"-"+month+"-"+date+" "+hours+":"+minutes;
											    	//获取头像、用户名
											    	var box=$('.user-name');
											    	//console.log(box.find('p').html()+" "+box.find('img').attr('src'));
													var my_lord='<div class="mjvp-content-child-news-nr"><div class="mjcontent-child-news-nr-img"><img class="comment-u-pic" title="" alt="" src="'+$('.user-img a').find('img').attr('src')+'"></div><div class="mjcontent-child-news-nr-body"><div class="mjcontent-child-news-nr-title"><span style="color:#0078b6;">'+out_time+' &mdash; <a title="查看用户资料" href="/?q=user/1">'+box.find('p').html()+'</a>：</span> <p>'+form.find('#edit-comment').val()+'</p><div class="mjcontent-child-news-nr-tm"><div class="mjhhf">回复</div></div></div></div></div>';
													my_box.after(my_lord);
													my_box.fadeOut('fast');
											    	//end




										  },
										  error: function(XMLHttpRequest, textStatus, errorThrown){
										  	console.log(XMLHttpRequest);
										  	console.log(textStatus);
										  	console.log(errorThrown);						
												alert('发生错误，1211111');
											}
										});
										
						  		}	   
								   
								  }),5000);
						  },
						  error: function(XMLHttpRequest, textStatus, errorThrown){
							alert('发生错误，请联系管理员');
							}
						});
				}else{
					c.html(fc.html()).addClass('vp_comment').removeClass('vp_share');
						/*c.find('#comment-form-1').submit(function(e){
								    e.preventDefault();
								    alert("Submit prevented");
								  });
						  	console.log(c.find('#edit-submit-1'));*/
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