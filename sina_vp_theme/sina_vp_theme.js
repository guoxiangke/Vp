// $Id: sina_open.js,v 1.2 2011/02/11 03:52:44 eastcn Exp $
Drupal.behaviors.vp_theme = function (context) {
	 $("#vp_search").attr('placeholder','搜索');
	 
	 //用户操作中心
	 $('.user-name').click(function(){
	    $('.xllist').slideToggle('medium');
	});
	
}
