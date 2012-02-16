// $Id: sina_open.js,v 1.2 2011/02/11 03:52:44 eastcn Exp $

Drupal.behaviors.sina = function (context) {
	
	
	    $('.filefield-upload input.form-submit').css("visibility", "hidden");
	    //$('.filefield-upload input.form-file').css("visibility", "hidden");
	    $('.filefield-upload input.form-file').change(function() {
	        var show = $(this).parent().find('input.form-submit').mousedown();
	    });
	    //发布微博时的 图片触发
	    
			$("#edit-sina-vp-open-wrapper").addClass('clear-block');			
			//$("#edit-field-weibo-image-0-upload-wrapper").addClass('hidden');
		 	$('#weibo_add_img').click(function(){
		 		$("#edit-field-weibo-image-0-upload-wrapper").find("#edit-field-weibo-image-0-upload").attr('dispaly','block').click();
		 	});
		 	
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
	
	function showFace() {
		$("#faceWrap").toggle();
		

		$(document.body).click(function(e) {
			$("#faceWrap").hide();
		});
		$(document.body).scroll(function(e) {
			$("#faceWrap").hide();
		});
	}
   
	

	function insertFace(showid, text) {
		var obj = document.getElementById(showid);
		//alert(obj.value);
		//alert(text);
		selection = document.selection;
		checkFocus(showid);
		if(!isUndefined(obj.selectionStart)) {
			var opn = obj.selectionStart + 0;
			obj.value = obj.value.substr(0, obj.selectionStart) + text + obj.value.substr(obj.selectionEnd);
		} else if(selection && selection.createRange) {
			var sel = selection.createRange();
			sel.text = text;
			try {
				sel.moveStart('character', -strlen(text));
			} catch(e) {
			}
		} else {
			obj.value += text;
		}
		//alert(obj.value);
		//alert(text);
		//var maxlen = 140;
		//if(USER.wb.pid == 1 && LIST == "dm") {
		//maxlen = 300;
		//}
		//checkText(showid, maxlen);
	}

	function isUndefined(variable) {
		return typeof variable == 'undefined' ? true : false;
	}

	function checkFocus(target) {
		var obj = document.getElementById(target);
		if(!obj.hasfocus) {
			obj.focus();
		}
	}

	function insertTopic(showid) {
		var topic = "请在这里输入自定义话题";

		var inputor = document.getElementById(showid);
		var hasCustomTopic = new RegExp('#请在这里输入自定义话题#').test(inputor.value);
		var text = topic, start = 0, end = 0;

		inputor.focus();

		if(document.selection) {
			var cr = document.selection.createRange();
			//获取选中的文本
			text = cr.text || topic;

			//内容有默认主题，且没选中文本
			if(text == topic && hasCustomTopic) {
				start = RegExp.leftContext.length + 1;
				end = topic.length;
			}
			//内容没有默认主题，且没选中文本
			else if(text == topic) {
				cr.text = '#' + topic + '#';
				start = inputor.value.indexOf('#' + topic + '#') + 1;
				end = topic.length;
			}
			//有选中文本
			else {
				cr.text = '#' + text + '#';
			}

			if(text == topic) {
				cr = inputor.createTextRange();
				cr.collapse();
				cr.moveStart('character', start);
				cr.moveEnd('character', end);
			}

			cr.select();
		} else if(inputor.selectionStart || inputor.selectionStart == '0') {
			start = inputor.selectionStart;
			end = inputor.selectionEnd;

			//获取选中的文本
			if(start != end) {
				text = inputor.value.substring(start, end);
			}

			//内容有默认主题，且没选中文本
			if(hasCustomTopic && text == topic) {
				start = RegExp.leftContext.length + 1;
				end = start + text.length;
			}
			//内容没有默认主题，且没选中文本
			else if(text == topic) {
				inputor.value = inputor.value.substring(0, start) + '#' + text + '#' + inputor.value.substring(end, inputor.value.length);
				start++;
				end = start + text.length;
			}
			//有选中文本
			else {
				inputor.value = inputor.value.substring(0, start) + '#' + text + '#' + inputor.value.substring(end, inputor.value.length);
				end = start = start + text.length + 2;
			}

			//设置选中范
			inputor.selectionStart = start;
			inputor.selectionEnd = end;
		} else {
			inputor.value += '#' + text + '#';
		}

	}


	$('a.facexy').click(function() { showFace();
		return false;
	});
	$('a.topic').click(function() {insertTopic('edit-title');
		return false;
	});
	$("#faceWrap a").click(function() {insertFace('edit-title', $(this).attr('title'));
		return false;
	});
}