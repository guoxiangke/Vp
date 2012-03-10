$().ready(function(){
	var box_width=parseInt('150px');
	var box_height=parseInt('150px');
	var flag=false;
	$('.thumb-image').mouseover(function(){
		flag=true;
		$(this).css("opacity","0.8");
	}).mouseout(function(){
		$(this).css("opacity","1");
	}).mousemove(function(event){
				if(flag==false){
				}else if(flag=true){
					//初始化数据
					var e =  event || window.event ;
					var obj=$(this);			
					//获取图片的展示宽高，
					var img_obj=new Image();
					img_obj.src=obj.attr('src');
					image_width=img_obj.width;
					image_height=img_obj.height;

					//获取鼠标坐标
					mouse_x=e.offsetX || e.originalEvent.layerX;
					mouse_y=e.offsetY || event.originalEvent.layerY;

					
					var get_mouse_x=obj.attr('mouse_x');
					var get_mouse_y=obj.attr('mouse_y');
					if(get_mouse_x==null){
						obj.attr('mouse_x',mouse_x);
						obj.attr('mouse_y',mouse_y);	
					}else{
						var mouse_move_x=mouse_x-get_mouse_x;
						var mouse_move_y=mouse_y-get_mouse_y;
						mouse_x=e.offsetX || e.originalEvent.layerX;
						mouse_y=e.offsetY || event.originalEvent.layerY;
						obj.attr('mouse_x',mouse_x);
						obj.attr('mouse_y',mouse_y);

						if(image_height/image_width>1){
							var limit_y=obj.attr('height')-box_height;
							var image_move_y=mouse_move_y;
							//image_move_y
							var image_margin_top=parseInt(obj.css('margin-top'))+image_move_y;
							

							
							if(-(limit_y)<image_margin_top && image_margin_top<0){
								
								obj.css('margin-top',image_margin_top);
								//alert(limit_y+" "+image_margin_top);
							}
						}else if(image_width/image_height>1){
							var limit_x=obj.attr('width')-box_width;
							var image_move_x=mouse_move_x;
							//image_move_y
							var image_margin_left=parseInt(obj.css('margin-left'))+image_move_x;
							

							
							if(-(limit_x)<image_margin_left && image_margin_left<0){
								
								obj.css('margin-left',image_margin_left);
								//alert(limit_y+" "+image_margin_top);
							}
						}
					}

														
					
									
				}
	}).click(function(e){
				
				var $obj=$(this);
					$obj.css('display',"none");
					$obj.parents('.status-box').find('.image_box').css('width','auto').css('height','auto');
					$obj.parents('.status-box').find('.normal-image-hidden').css('display','block');
					$obj.parents('.status-box').find('.thumb-image').css('display','none');
					var y=$obj.parent().offset().top;
					//parseInt($obj.parent().css('height'))+
					var x=0;
					window.scrollBy(x,y);
					//alert('end');
			}).load(function(){
				var obj=$(this);
				var image_orignal_height=obj.height();
				var image_orignal_width=obj.width();

				if(image_orignal_width>box_width || image_orignal_height>box_height){
					if(image_orignal_height/image_orignal_width>1){
						var limit_size=image_orignal_width/box_width;
						obj.attr('width',box_width);
						obj.attr('height',image_orignal_height/limit_size);

					}else if(image_orignal_height/image_orignal_width<=1){
						var limit_size=image_orignal_height/box_height;
						obj.attr('height',box_height);
						obj.attr('width',image_orignal_width/limit_size);					
					}
				}

			})
			$('.normal-image-hidden').click(function(e){
				var $obj=$(this);
				var visiable_flag=$obj.css('display');
				if(visiable_flag=='block'){
					
					$obj.parents('.status-box').find('.normal-image-hidden').css('display','none');
					$obj.parents('.status-box').find('.image_box').css('width',box_width+'px').css('height',box_height+'px');
					$obj.parents('.status-box').find('.thumb-image').css('display','block');
					var y=$obj.position().top-$obj.parent().offset().top;
					//alert(y);
					
					window.scrollBy(0,-y);
				}
			
			})
		
})