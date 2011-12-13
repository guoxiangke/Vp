// $Id: sina_open.js,v 1.2 2011/02/11 03:52:44 eastcn Exp $

Drupal.behaviors.sina11 = function (context) {
	hs.graphicsDir = 'sites/all/modules/VP/sina_vp/images/highslide/graphics/';
	if($(".imagecache-vp_imagelink").length>0){
		var classes = $("a.imagecache-vp_imagelink").attr("class")+" highslide";
		$("a.imagecache-vp_imagelink").attr("class",classes);	
		$("a.imagecache-vp_imagelink").attr("onclick","return hs.expand(this)");
	}
}