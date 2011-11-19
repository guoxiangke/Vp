// $Id: sina_open.js,v 1.2 2011/02/11 03:52:44 eastcn Exp $

Drupal.behaviors.sina = function (context) {
	//hs.graphicsDir = 'graphics/';
	var classes = $("a.imagecache-vp_imagelink").attr("class")+" highslide";
	alert(classes);
	$("a.imagecache-vp_imagelink").attr("class",classes);	
	$("a.imagecache-vp_imagelink").attr("onclick","return hs.expand(this)");
}