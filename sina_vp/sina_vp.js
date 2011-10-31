// $Id: sina_open.js,v 1.2 2011/02/11 03:52:44 eastcn Exp $

Drupal.behaviors.sina = function (context) {
	
	function c() {
		var _c = 140;
		var b = $('#edit-title').val();
		if (b.length < _c) {
			_c -= b.length;
		} else if (b.length > _c) {
			_c -= b.length;
			//alert('太长了');
		} else {
			_c = 0;
		}
		$('#sina_open_tweet_text_count').text(_c);
	};
	
	c();
	
	$('#edit-title').bind('keyup', c);
	$('#edit-title').bind('mouseup', c);
}