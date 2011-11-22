// $Id: sina_open.js,v 1.2 2011/02/11 03:52:44 eastcn Exp $

Drupal.behaviors.sina_vp = function (context) {
	$("img.imagecache-vp").lazyload({
			placeholder : "sites/all/modules/VP/sina_vp/images/grey.gif",
			effect : "fadeIn" 
		});
}