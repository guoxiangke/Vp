$(document).ready(function(){
	var box_width=parseInt('150px');
	var box_height=parseInt('150px');
	var flag=false;
	$('.thumb-image').mouseover(function(){
		flag=true;
		$(this).css("opacity","0.8");
	}).mouseout(function(){
		$(this).css("opacity","1");
	}).click(function(e){
				
				var $obj=$(this);
					$obj.css('display',"none");
					$obj.parents('.status-box').find('.image_box').css('width','auto').css('height','auto');
					$obj.parents('.status-box').find('.normal-image-hidden').css('display','block');
					$obj.parents('.status-box').find('.thumb-image').css('display','none');

					var x=0;
					var y=$(this).parent().offset().top;
					//window.scrollBy(x,y);
					//alert('end');
					
			}).load(function(){
				var obj=$(this);
				var image_orignal_height=obj.height();
				var image_orignal_width=obj.width();

				if(image_orignal_width>box_width || image_orignal_height>box_height){
					if(image_orignal_height/image_orignal_width>1){
						obj.css('max-width',box_width);
					}else if(image_orignal_height/image_orignal_width<=1){
						obj.css('max-height',box_height);
					}
				}

			}).mousemove(function(event){
				if(flag==false){
				}else if(flag=true){
					//初始化数据
					var e =  event || window.event ;
					var obj=$(this);			
					//获取图片的展示宽高，
					image_width=obj.width();
					image_height=obj.height();
					//获取图片真实的宽高
					var img_obj=new Image();
					img_obj.src=obj.attr('src');
					image_real_width=img_obj.width;
					image_real_height=img_obj.height;
				}
					//获取鼠标坐标
					mouse_x=e.offsetX || e.originalEvent.layerX;
					mouse_y=e.offsetY || event.originalEvent.layerY;
					var get_mouse_x=obj.attr('mouse_x');
					var get_mouse_y=obj.attr('mouse_y');
					if(get_mouse_x==null){
						obj.attr('mouse_x',mouse_x);
						obj.attr('mouse_y',mouse_y);	
					}else{
						var mouse_move_x=-(mouse_x-get_mouse_x);
						var mouse_move_y=-(mouse_y-get_mouse_y);
						mouse_x=e.offsetX || e.originalEvent.layerX;
						mouse_y=e.offsetY || event.originalEvent.layerY;
						obj.attr('mouse_x',mouse_x);
						obj.attr('mouse_y',mouse_y);
					
					if(image_height/image_width>1){
						var limit_y=obj.height()-box_height;
						var image_move_y=mouse_move_y*(image_real_height/box_height);
						var image_margin_top=parseInt(obj.css('margin-top'))+image_move_y;
							

							
						if(-(limit_y)<image_margin_top && image_margin_top<0){
								
							obj.css('margin-top',image_margin_top);
							//alert(limit_y+" "+image_margin_top);
						}
					}else if(image_width/image_height>1){
						var limit_x=obj.width()-box_width;
						var image_move_x=mouse_move_x*(image_real_width/box_width);
						//image_move_y
						var image_margin_left=parseInt(obj.css('margin-left'))+image_move_x;
							

							
						if(-(limit_x)<image_margin_left && image_margin_left<0){
								
							obj.css('margin-left',image_margin_left);
							//alert(limit_y+" "+image_margin_top);
						}
					}

					}
			})
			$('.normal-image-hidden').click(function(e){
				var $obj=$(this);
				var obj=$obj;
				var visiable_flag=$obj.css('display');
				if(visiable_flag=='block'){
					
					$obj.parents('.status-box').find('.normal-image-hidden').css('display','none');
					$obj.parents('.status-box').find('.image_box').css('width',box_width+'px').css('height',box_height+'px');
					$obj.parents('.status-box').find('.thumb-image').css('display','block');
					var screenHeight = $(window).height(), screenHeight = $(window).height(); //当前浏览器窗口的 宽高
					var scrolltop = $(document).scrollTop();//获取当前窗口距离页面顶部高度

					var objTop = (screenHeight - obj.height())/2 + scrolltop;
					var pare_pos=$(this).parent().offset().top;

				}
			 
			})

		// ajax plaza begin by dale 
		/*var curr_p_type = 'Activity';//第一个显示的是真人秀。
				$('.gcvp-nav').click(function(){
					var curr_p_type = $('.showed').attr('p-type');
					var p_type = $(this).attr('p-type');
					var fc = $('.gcvp-body-right');
					if(fc.find('.posted').hasClass('.'+p_type)){ //当已经post以后，就不ajax了
						console.log(fc.find('.posted').find('.'+p_type));
					}
					{
						$.ajax({
							url:  $(this).children('a').attr("href")+'/json',
							type: 'POST',
							data: '',
							dataType: 'json',
						  	success: function(json) {
						  	fc.children('.gcbody-right-content').not('.posted').addClass(curr_p_type).addClass('posted').fadeOut();
						  	fc.append(json.data).fadeIn();
						  	//c.html(fc.html()).addClass('vp_comment').removeClass('vp_share');
						  	//var form = c.find('form')
						  },
						  error: function(XMLHttpRequest, textStatus, errorThrown){
								alert('发生错误，请联系管理员');
							}
						});
					}
					
				});	
		//end*/
		//左部效果
		$('.gcvp-nav').css('cursor','pointer');
		$('.gcvp-sidebar').ready(function(){
			$('.gcvp-subnav').css('display','none');
			
				//0316URL解析
			/*	
				//广场频道：新品 活动 爆款 特卖  真人秀 转让潮
				$plaza_types = array('News','Activity','Special','Sale','Show','Transfer');
				//广场子频道： 最近更新 评论最多 喜欢最多
				$plaza_child_types = array('recently_active','most_comments','most_favor');
			*/
				thisURL = document.URL; 
				var my_way=thisURL.split('/');
				var my_array=new Array();
				for(var i=0;i<=2;i++){
					my_array[i]=my_way[i+3];
				}
				if(my_array[2]==undefined){
					my_array[2]=='base';
				}
				//console.log(my_array);
				//在这里添加工厂处理。
				if(my_array[0]=="?q=plaza"){
					changeColor(my_array[1],my_array[2]);
				}
				function changeColor(box,inner){
					/**
					*
					*/
					switch (box){
						case "News"	:
							//console.log('News 执行了');
							$('.gcvp-nav[p-type=News]').addClass('showed');
							$('.gcvp-nav[p-type=News]').css('background-color','#a1a1a1');
							$('.gcvp-nav[p-type=News]').css('color','#FFFFFF');
							$('.gcvp-nav[p-type=News]').next().show().addClass('showed');
							break;
						case "Activity"	:
							//console.log('Activity 执行了');
							$('.gcvp-nav[p-type=Activity]').addClass('showed');
							$('.gcvp-nav[p-type=Activity]').css('background-color','#a1a1a1');
							$('.gcvp-nav[p-type=Activity]').css('color','#FFFFFF');
							$('.gcvp-nav[p-type=Activity]').next().show().addClass('showed');
							break;	
						case "Special"	:
							//console.log('Special 执行了');
							$('.gcvp-nav[p-type=Special]').addClass('showed');
							$('.gcvp-nav[p-type=Special]').css('background-color','#a1a1a1');
							$('.gcvp-nav[p-type=Special]').css('color','#FFFFFF');
							$('.gcvp-nav[p-type=Special]').next().show().addClass('showed');
							break;		
						case "Sale"	:
						//	console.log('Special 执行了');
							$('.gcvp-nav[p-type=Sale]').addClass('showed');
							$('.gcvp-nav[p-type=Sale]').css('background-color','#a1a1a1');
							$('.gcvp-nav[p-type=Sale]').css('color','#FFFFFF');
							$('.gcvp-nav[p-type=Sale]').next().show().addClass('showed');
							break;
						case "Show"	:
						//	console.log('Special 执行了');
							$('.gcvp-nav[p-type=Show]').addClass('showed');
							$('.gcvp-nav[p-type=Show]').css('background-color','#a1a1a1');
							$('.gcvp-nav[p-type=Show]').css('color','#FFFFFF');
							$('.gcvp-nav[p-type=Show]').next().show().addClass('showed');
							break;
						case "Transfer"	:
						//	console.log('Special 执行了');
							$('.gcvp-nav[p-type=Transfer]').addClass('showed');
							$('.gcvp-nav[p-type=Transfer]').css('background-color','#a1a1a1');
							$('.gcvp-nav[p-type=Transfer]').css('color','#FFFFFF');
							$('.gcvp-nav[p-type=Transfer]').next().show().addClass('showed');
							break;
						default	: 'ok';
					}
					switch (inner){
						case "recently_active":
						//	console.log('recently_active 执行了');
							$('.gcvp-subnav .active').css('color','#00CBFF');
						case "most_comments":
						//	console.log('most_comments 执行了');
							$('.gcvp-subnav .active').css('color','#00CBFF');
						case "most_favor":
						//	console.log('most_favor 执行了');
							$('.gcvp-subnav .active').css('color','#00CBFF');
						default	: 'ok';
					}
					//$('.gcvp-nav:first').addClass('showed');
					//$('.gcvp-subnav:first').show();
					//$('.gcvp-nav:first').css('background-color','#a1a1a1');
					//$('.gcvp-nav:first').css('color','#FFFFFF');
				}

			
			$('.gcvp-subnav p').css('cursor','pointer');
		})
		$('.gcvp-nav').click(function(){
			var address=$(this).find('a').attr('href');
			window.location=address;
		})
		//link-Color:#00CBFF rgb(0,203,255)
		//selected-color:#999999 rgb(153,153,153);
		$('.gcvp-subnav p').click(function(){
				var myColor=$(this).css('color');
				if(myColor=="rgb(153, 153, 153)"){
					$(this).parents().find('.gcvp-subnav p').css('color','rgb(153, 153, 153)');
					$(this).css('color','rgb(0, 203, 255)');
				}
		})
		
		//ajax_comment_divDialog_demo
		//这里增加延时处理
		/**
		*2012年3月19日 p.m.
		 2012年3月20日 p.m.-》增加鼠标判断事件。
		*/
		$('.close_box').attr('href','javascript:void(0)');

		$('.gcfudiv').hover(
			function(){
				$(this).attr('mouseIn','in');
			},
			function(){
				$(this).attr('mouseIn','out');
				clearTimeout(signo);
				var signo=setTimeout(function(){
					if($(this).hasClass('dialogIn') && $(this).attr('mouseIn')=='out'){
						$(this).fadeOut('fast');
						$(this).removeClass('dialogIn');
					}
				},10000)
			}
		)

		$('.gcfudivbtn').click(function(){
			var textSel=$(this).parent().find('.gcfudivtext');
			var word=textSel.val();
			if(word.length==0 || word=='求点评^_^' || word=="评论已送出!!"){

			}else{
				var userName=$('.user-name p').text();
				var userPic=$('.user-img img').attr('src');		
				var words="<span class='gcvp-name'>"+userName+"：</span>"+word;
				var hideThat=$(this).parent().next().find('p');//.fadeOut('slow',function(){$(this).html(words)});
				var hideThis=$(this).parent().next().find('.hideMe');
				var pTar=$(this).parent().next().find('p');
				var imgTar=$(this).parent().next().find('.hideMe');
				$(this).parent().find('.hidden').find('textarea').val(word);
				var url=$(this).parent().find('.hidden').find('form').attr('action');
				var form=$(this).parent().find('.hidden').find('form');
				//var disCount=$(this).
				//
				thisHost = location.hostname;
				url=thisHost+url;
				var discuz=$(this).parent().parent().parent().prev().find('.gcplfx a:first');
				//var disCount=parseInt(discuz.text())+1;
				var disCount=parseInt(discuz.text().substr(2))+1;
				
				var button=$(this);
				button.attr('disabled','disabled');
				$.ajax({
					url:url,
					type:"POST",
					data:form.serialize(),
					success:function(){
						hideThat.fadeOut('slow',function(){$(this).html(words)});
						hideThis.fadeOut('slow',function(){$(this).attr('src',userPic)});
						pTar.fadeIn('slow');
						imgTar.fadeIn('slow');
						discuz.text('评论'+disCount);
						textSel.val("评论已送出!!");
						button.removeAttr('disabled');
					}

				})
			}
		})
		$('.gcfudivtext').click(function(){
			$(this).val('');
		})
		$('.close_box').click(function(){
			$(this).parent().parent().fadeOut();
			$(this).parent().parent().parent().find('.gc-pl-sanjiao').fadeOut();
			$(this).parent().parent().removeClass('dialogIn');
		})




		//这里用于换一批效果
		//0316换一批代码优化
		//去掉轮播效果，下一批到底不动
		//将分类的其他两个栏目也加入这个效果
		$('.vp-rec-wrap').css('position','absolute');
		//$('.vp-rec-wrap').parent().css('overflow','hidden');
		//$('.vp-rec-wrap').parent().parent().css('overflow','hidden');
		$('.vp-rec-wrap').css('width','675px');
		$('.vp-rec-wrap').css('display','none');
		$('.vp-rec-wrap:first').css('display','block');
		$('#vp_recommend_focus .vp-rec-wrap:last').addClass('last');
		$('#vp_recommend_new').hide();
		$('#vp_recommend_active').hide();
		$('#vp_recommend_new .vp-rec-wrap:last').addClass('last');
		$('#vp_recommend_active .vp-rec-wrap:last').addClass('last');
		$('.dpvp-more a').click(function(){
			if($(this).parent().parent().hasClass('last')){
				$(this).css('color','#CCCCCC');
			}else{
				$(this).parent().parent().fadeOut('fast',function(){$(this).css('left','337.5px');$(this).hide();});
				$(this).parent().parent().next().fadeIn('fast');
			}
				
			
		})
/**
*分类效果备份
	$('.dpright-content-top .nav-tabs li a').click(function(){
		if($(this).parent().attr('class')=='last-li' || $(this).parent().attr('class')=='active'){	
			}else{
				//实现toggleClass效果
				$(this).parent().parent().find('.active').removeClass('active');
				$(this).parent().addClass('active');
				var flag=$(this).attr('mylist');
				//实现切屏幕效果
				$('.vp_recommend_scroll').hide();
				$('.vp-rec-wrap').hide();
			//	alert('#'+flag);
				$('#'+flag).fadeIn('fast');
				$('#'+flag).children('.vp-rec-wrap:first').fadeIn('fast');
			}
		})



*备份结束
*/


//浮动字体效果

	$('.gcvp-body-bottom-pk-right-img img ~ div').hide();
	$('.gcvp-body-bottom-pk-right-img').hover(
		function(){
			$(this).find('div').show();
		},
		function(){
			$(this).find('div').fadeOut();
		}
	).mousemove(function(){
		$(this).find('div').show();
	})

})