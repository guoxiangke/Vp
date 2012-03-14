$().ready(function(){
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

		// ajax dale 
		var curr_p_type = 'Activity';//第一个显示的是真人秀。
				$('.gcvp-nav').click(function(){
					var curr_p_type = $('.showed').attr('p-type');
					//var p_type = $(this).attr('p-type');
					var fc = $('.gcvp-body-right');
					//if(fc)
					$.ajax({
						url:  $(this).children('a').attr("href")+'/json',
						type: 'POST',
						data: '',
						dataType: 'json',
					  success: function(json) {
					  	fc.children('.gcbody-right-content').addClass(curr_p_type).addClass('posted').fadeOut();
					  	fc.append(json.data).fadeIn();
					  	//c.html(fc.html()).addClass('vp_comment').removeClass('vp_share');
					  	//var form = c.find('form')
					  },
					  error: function(XMLHttpRequest, textStatus, errorThrown){
							alert('发生错误，请联系管理员');
						}
					});
				});	

		//左部效果
		$('.gcvp-nav').css('cursor','pointer');
		$('.gcvp-sidebar').ready(function(){
			$('.gcvp-subnav').css('display','none');
			$('.gcvp-subnav:first').show();
			$('.gcvp-nav:first').css('background-color','#a1a1a1');
			$('.gcvp-nav:first').css('color','#FFFFFF');
			$('.gcvp-nav:first').addClass('showed');
			$('.gcvp-subnav p').css('cursor','pointer');
		})
		$('.gcvp-nav').click(function(){
			
			if($(this).hasClass('showed')){
				
			}else{
				$('.showed').css('background-color','#F0F0F0');
				$('.showed').next().hide('200');
				$('.showed').css('color','#A1A1A1');
				$('.showed').removeClass('showed');
				$(this).addClass('showed');
				$(this).css('background-color','#A1A1A1').css('color','#FFFFFF');
				$(this).next().find('p:first').css('color','rgb(0, 203, 255)');
				$(this).next().show('200');
			}
			
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

		
})