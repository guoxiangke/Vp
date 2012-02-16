// $Id: sina_open.js,v 1.2 2011/02/11 03:52:44 eastcn Exp $

	function showLoadingImg() {
	  $('#showmore').html('<a class="handle" href="javascript:show()"><img src="sites/all/modules/VP/sina_vp/images/loading.gif" height="32px" />显示更多结果</a>');
	}
	 function insertcode() {//// & pid=' + id + ' & remark='+$('.enroll-form-remark').val(),
	      showLoadingImg();
	      $("#showmore_node").show();
	      var post_url = $(".pager-current").next().find('a.active').attr('href');
	      if(!post_url){ return false};
	      if($(".pager-current").next()== $(".pager-last") ) {return false};
	      
	      $.ajax({
							url: post_url,
							type: 'POST',
							data: '',
							dataType: 'html',
						  success: function(data) {
						  	$('#showmore').hide();
						  	$current = $('.pager .pager-current').removeClass('pager-current').addClass('pager-item').next().addClass('pager-current');
						  	//console.log(XMLHttpRequest.responseText);
						  	var nodeList = $(data).find('#tabs-wrapper').next();
						  	nodeList.find('.pager').remove()
						  	//$('.pager').hide();
								data = nodeList;
						  	$("#showmore").after(data).remove();
								
						  },
						  error: function(XMLHttpRequest, textStatus, errorThrown){
								 console.log(XMLHttpRequest.responseText);
									//alert('发生错误，请联系管理员1');
							}
						});
	      $("#showmore_node").html('<div style="height:10px;width:100%;border:1px ">在这里填充数据。</div>');
	      //showLoadingText();
	  }
	  $(document).ready(function () {
			  $("#showmore").click(function() {
						      //insertcode();
						});	

	      $(window).scroll(function () {
	          var $body = $("body");
	          var $html = "";
	          $html += "<br/>" + ($(window).height() + $(window).scrollTop());
	          $html += "<br/>window.height: " + $(window).height();
	          $html += "<br/>body.height: " + $body.height();
	          $html += "<br/>window.scrollTop: " + $(window).scrollTop();
	          $("#page_tag_bottom").html($html);
	    /*判断窗体高度与竖向滚动位移大小相加 是否 超过内容页高度*/
	          if (($(window).height() + $(window).scrollTop()) >= $body.height()) {
	              //$("#showmore_node").show();
	              //setTimeout(insertcode, 1000);/*IE 不支持*/
	              insertcode();
	          }
	      });
	    
	  });
