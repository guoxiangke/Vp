function htmlspecialchars (string, quote_style, charset, double_encode) {
    var optTemp = 0,
        i = 0,
        noquotes = false;
    if (typeof quote_style === 'undefined' || quote_style === null) {
        quote_style = 2;
    }
    string = string.toString();
    if (double_encode !== false) { // Put this first to avoid double-encoding
        string = string.replace(/&/g, '&amp;');
    }
    string = string.replace(/</g, '&lt;').replace(/>/g, '&gt;');
 
    var OPTS = {
        'ENT_NOQUOTES': 0,
        'ENT_HTML_QUOTE_SINGLE': 1,
        'ENT_HTML_QUOTE_DOUBLE': 2,
        'ENT_COMPAT': 2,
        'ENT_QUOTES': 3,
        'ENT_IGNORE': 4
    };
    if (quote_style === 0) {
        noquotes = true;
    }
    if (typeof quote_style !== 'number') { // Allow for a single string or an array of string flags
        quote_style = [].concat(quote_style);
        for (i = 0; i < quote_style.length; i++) {
            // Resolve string input to bitwise e.g. 'ENT_IGNORE' becomes 4
            if (OPTS[quote_style[i]] === 0) {
                noquotes = true;
            }
            else if (OPTS[quote_style[i]]) {
                optTemp = optTemp | OPTS[quote_style[i]];
            }
        }
        quote_style = optTemp;
    }
    if (quote_style & OPTS.ENT_HTML_QUOTE_SINGLE) {
        string = string.replace(/'/g, '&#039;');
    }
    if (!noquotes) {
        string = string.replace(/"/g, '&quot;');
    }
 
    return string;
}
// in_array('van', ['Kevin', 'van', 'Zonneveld']);
function in_array (needle, haystack, argStrict) {
    var key = '',
        strict = !! argStrict;
 
    if (strict) {
        for (key in haystack) {
            if (haystack[key] === needle) {
                return true;
            }
        }
    } else {
        for (key in haystack) {
            if (haystack[key] == needle) {
                return true;
            }
        }
    }
 
    return false;
}


//生成定时选择器
function init_select_time(pre) {
	var html = "", _d = new Date(), y = _d.getFullYear(), y2 = y, m = _d.getMonth() + 1, d = _d.getDate(), h = _d.getHours(), i = _d.getMinutes();
		
	var day_count = 31;
	if(m==1||m==3||m==5||m==7||m==8||m==10||m==12) {
		day_count = 31;
	} else if(m==4||m==6||m==9||m==11) {
		day_count = 30;
	} else {
		if(y%4==0) {
			day_count = 29;
		} else {
			day_count = 28;	
		}
	}
	
	//d++; //第二天凌晨
	i += 30; //30分钟后
	if(i > 59) {
		h++;
		i -= 59;
		if(h > 23) {
			d++;
			h -= 23;
		}
	}	
	
	if (d > day_count) {
		m++;
		d = 1;
		
		if (m > 12) {
			m = 1;
			y2++;
		}
	}
	
	html += '<select name="' + pre + '_y" id="' + pre + '_y">';
	for(var i1=0; i1<5; i1++) {
		html += '<option value="'+(y+i1)+'"' +((y+i1)==y2?' selected="selected"':'')+ '>'+(y+i1)+'</option>';
	}
	html += '</select> 年 ';
	
	//alert(m);
	
	html += '<select name="' + pre + '_m" id="' + pre + '_m">';
	for(var i2=1; i2<13; i2++) {
		html += '<option value="'+i2+'"' +(i2==m?' selected="selected"':'')+ '>'+i2+'</option>';
	}
	html += '</select> 月 ';
	
	html += '<select name="' + pre + '_d" id="' + pre + '_d">';
	for(var i3=1; i3<32; i3++) {
		html += '<option value="'+i3+'"' +(i3==d?' selected="selected"':'')+ '>'+i3+'</option>';
	}
	html += '</select> 日 ';
	
	html += '<select name="' + pre + '_h" id="' + pre + '_h">';
	for(var i4=0; i4<24; i4++) {
		html += '<option value="'+i4+'"' +(i4==h?' selected="selected"':'')+ '>'+i4+'</option>';
	}
	html += '</select> 时 ';
	
	html += '<select name="' + pre + '_i" id="' + pre + '_i">';
	for(var i5=0; i5<60; i5++) {
		html += '<option value="'+i5+'"' +(i5==i?' selected="selected"':'')+ '>'+i5+'</option>';
	}
	html += '</select> 分';
	
	return html;
}

function parseSourceHtml(source) {
	return isUndefined(source)?'':'来自<a href="' + source.url + '" target="_blank">' + source.text + '</a>';
}

function parseTimeHtml(timestamp, pid, id, user_id) {
	if(isUndefined(timestamp)) return "";
	else return '<a class="time" target="_blank" href="/?c=go&pid=' + pid + '&id=' + id + '&uid=' + user_id + '" rel="' + timestamp + '" title="' + formatTime(new Date(timestamp*1000), "YYYY年MM月DD日 HH:MS") + '">' + parseTime(timestamp) + '</a> ';
}

function parseTime(timestamp) {
	//if(isUndefined(timestamp)) return "";
	
	var now = new Date();
	var diff = Math.floor(now.getTime()/1000) - timestamp;
	
	if(diff < 60) {
		return diff + "秒前";
	} else if (diff < 3600) {
		return Math.ceil(diff / 60) + "分钟前";
	}
	
	var date = new Date(timestamp*1000);
	
	if (date.getFullYear() === now.getFullYear() && date.getMonth() === now.getMonth() && date.getDate() === now.getDate()) {
		return formatTime(date, "今天 HH:MS");
	}
	if (date.getFullYear() === now.getFullYear() && date.getMonth() === now.getMonth() && date.getDate() === now.getDate()-1) {
		return formatTime(date, "昨天 HH:MS");
	}
	
	return formatTime(date, "MM月DD日 HH:MS");
}

function formatTime(d, f) {
	if (d == null) {
		return "";
	}

	f = f.replace(/YYYY/g, d.getFullYear());
	f = f.replace(/MM/g, addZero((d.getMonth() + 1)));
	f = f.replace(/DD/g, addZero(d.getDate()));
	f = f.replace(/HH/g, addZero(d.getHours()));
	f = f.replace(/MS/g, addZero(d.getMinutes()));
	f = f.replace(/SS/g, addZero(d.getSeconds()));
	
	return f;
}

function addZero(a) {
	if (a <= 9 && a >= 0) {
		return "0" + a;
	} else {
		return a;	
	}
}

function show_tip(tip, css) {
	var cls = css ? css : 'errmsg';
	
	$('#tipWrap').html("");
    $('<span class="' + cls+ '">' + tip + '</span>').appendTo('#tipWrap').fadeIn('slow').animate({opacity: 1.0}, 2000).fadeOut('slow', function() { $(this).remove(); });
}


function checkText(id, count) {
	var v = $.trim( $('#' + id).val() );
	var left = calWbText(v, count);
	if (left >= 0)
		$('#' + id + "_warn").html('还能输入<em>'+left+'</em>字');
	else
		$('#' + id + "_warn").html('已超出<em style="color:red;">'+Math.abs(left)+'</em>字');
			
	return left>=0 && v;
}

function calWbText(text, count) {
	var cLen=0;
	var matcher = text.match(/[^\x00-\xff]/g), wlen  = (matcher && matcher.length) || 0;
	return Math.floor((count*2 - text.length - wlen)/2);
}

function good() {
	//$('#status').focus().val("我正在使用享拍微博通，更新微博更轻松！不但可以同步多个微博（包括新浪微博、腾讯微博、搜狐微博等等）；还能查看、转发、评论、收藏、发私信等操作，大家快去看看吧！下载地址 http://www.wbto.cn");
	checkText("status", 140);
}

function insertTopic(){
	if(!topic) topic = "请在这里输入自定义话题";
	
	var inputor = document.getElementById('edit-title');
	var hasCustomTopic = new RegExp('#请在这里输入自定义话题#').test(inputor.value);
	var text = topic, start=0,end=0;
	
	inputor.focus();
	
	if (document.selection) {
		var cr = document.selection.createRange();
		//获取选中的文本
		text = cr.text || topic;
	
		//内容有默认主题，且没选中文本
		if (text == topic && hasCustomTopic) {
			start = RegExp.leftContext.length + 1;
			end   =   topic.length;
		}
		//内容没有默认主题，且没选中文本
		else if(text == topic) {
			cr.text = '#' + topic + '#';
			start = inputor.value.indexOf('#' + topic + '#') + 1;
			end   = topic.length;
		}
		//有选中文本
		else {
			cr.text = '#' + text + '#';
		}
	
		if (text == topic) {
			cr = inputor.createTextRange();
			cr.collapse();
			cr.moveStart('character', start);
			cr.moveEnd('character', end);
		}
	
		cr.select();
	}
	else if (inputor.selectionStart || inputor.selectionStart == '0') {
		start = inputor.selectionStart;
		end = inputor.selectionEnd;
	
		//获取选中的文本
		if (start != end) {
			text = inputor.value.substring(start, end);
		}
	
		//内容有默认主题，且没选中文本
		if (hasCustomTopic && text == topic) {
			start = RegExp.leftContext.length + 1;
			end = start + text.length;
		}
		//内容没有默认主题，且没选中文本
		else if (text == topic) {
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
	}
	else {
		inputor.value += '#' + text + '#';
	}
	
	//checkText("status", 140);
}


function showFace(showid, xy, type) {
	$("#faceWrap").toggle();//slideToggle
	
	var offset = $("#" + xy).offset();
	$("#faceWrap").css("left", offset.left+type-292).css("top", offset.top + $("#" + xy).height()-58+$("#content").scrollTop());
	
	//if($("#faceWrap").html() == "") {
		$.get("/source/face.php", function(data){
			$("#faceWrap").html(data);
			
			$("#faceWrap a").click(function(){
				insertFace(showid, $(this).attr("title"));						   
			});
		});
	//}
	
	//var offset = $("#"+showid+"_ps").offset();
	//$("#"+showid+"_face").css({top:offset.top-48,left:offset.left-292});
	
	doane();
	
	$(document.body).click(function(e) {
		$("#faceWrap").hide();
	});
	$(document.body).scroll(function(e) {
		$("#faceWrap").hide();
	});
	
}

function doane(event) {
	e = event ? event : window.event;
	if(!e) e = getEvent();
	if(e && $.browser.msie) {
		e.returnValue = false;
		e.cancelBubble = true;
	} else if(e) {
		e.stopPropagation();
		e.preventDefault();
	}
}
function getEvent() {
	if(document.all) return window.event;
	func = getEvent.caller;
	while(func != null) {
		var arg0 = func.arguments[0];
		if (arg0) {
			if((arg0.constructor  == Event || arg0.constructor == MouseEvent) || (typeof(arg0) == "object" && arg0.preventDefault && arg0.stopPropagation)) {
				return arg0;
			}
		}
		func=func.caller;
	}
	return null;
}


function insertFace(showid, text) {
	var obj = document.getElementById(showid);
	selection = document.selection;
	checkFocus(showid);
	if(!isUndefined(obj.selectionStart)) {
		var opn = obj.selectionStart + 0;
		obj.value = obj.value.substr(0, obj.selectionStart) + text + obj.value.substr(obj.selectionEnd);
	} else if(selection && selection.createRange) {
		var sel = selection.createRange();
		sel.text = text;
		try{sel.moveStart('character', -strlen(text));}
		catch(e){}
	} else {
		obj.value += text;
	}
	
	var maxlen = 140;
	if(USER.wb.pid == 1 && LIST == "dm") {
		//maxlen = 300;
	}
	checkText(showid, maxlen);
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

//多选，如onclick="selectAll(this, 'id')"
function selectAll(obj, chk) {
	if(chk == null) chk = 'checkboxes';
	var elems = obj.form.getElementsByTagName("INPUT");
	for(var i=0; i < elems.length; i++) {
		if(elems[i].name == chk || elems[i].name == chk + "[]") elems[i].checked = obj.checked;
	}
}


function un_link(str) {
	if(!str) return "";
	
	var r = /<\/?a[^>]*?>/ig;
	return str.replace(r, '');
}


function isTouchDevice() {
    try {
        document.createEvent("TouchEvent");
        return true;
    } catch(e) {
        return false;
    }
}
function touchScroll(id) {
    if (isTouchDevice()) {
        var el = document.getElementById(id);
        var scrollStartPos = 0;

        document.getElementById(id).addEventListener("touchstart", function(event) {
				scrollStartPos = this.scrollTop + event.touches[0].pageY;
				//event.preventDefault();
			},
			false);

        document.getElementById(id).addEventListener("touchmove", function(event) {
				this.scrollTop = scrollStartPos - event.touches[0].pageY;
				event.preventDefault();
			},
			false);
    }
}

function str_replace (search, replace, subject, count) {
    var i = 0, j = 0, temp = '', repl = '', sl = 0, fl = 0,
            f = [].concat(search),
            r = [].concat(replace),
            s = subject,
            ra = r instanceof Array, sa = s instanceof Array;
    s = [].concat(s);
    if (count) {
        this.window[count] = 0;
    }
 
    for (i=0, sl=s.length; i < sl; i++) {
        if (s[i] === '') {
            continue;
        }
        for (j=0, fl=f.length; j < fl; j++) {
            temp = s[i]+'';
            repl = ra ? (r[j] !== undefined ? r[j] : '') : r[0];
            s[i] = (temp).split(f[j]).join(repl);
            if (count && s[i] !== temp) {
                this.window[count] += (temp.length-s[i].length)/f[j].length;}
        }
    }
    return sa ? s : s[0];
}


(function($){
	
	var keyString = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	
	var uTF8Encode = function(string) {
		string = string.replace(/\x0d\x0a/g, "\x0a");
		var output = "";
		for (var n = 0; n < string.length; n++) {
			var c = string.charCodeAt(n);
			if (c < 128) {
				output += String.fromCharCode(c);
			} else if ((c > 127) && (c < 2048)) {
				output += String.fromCharCode((c >> 6) | 192);
				output += String.fromCharCode((c & 63) | 128);
			} else {
				output += String.fromCharCode((c >> 12) | 224);
				output += String.fromCharCode(((c >> 6) & 63) | 128);
				output += String.fromCharCode((c & 63) | 128);
			}
		}
		return output;
	};
	
	var uTF8Decode = function(input) {
		var string = "";
		var i = 0;
		var c = c1 = c2 = 0;
		while ( i < input.length ) {
			c = input.charCodeAt(i);
			if (c < 128) {
				string += String.fromCharCode(c);
				i++;
			} else if ((c > 191) && (c < 224)) {
				c2 = input.charCodeAt(i+1);
				string += String.fromCharCode(((c & 31) << 6) | (c2 & 63));
				i += 2;
			} else {
				c2 = input.charCodeAt(i+1);
				c3 = input.charCodeAt(i+2);
				string += String.fromCharCode(((c & 15) << 12) | ((c2 & 63) << 6) | (c3 & 63));
				i += 3;
			}
		}
		return string;
	}
	
	$.extend({
		base64Encode: function(input) {
			var output = "";
			var chr1, chr2, chr3, enc1, enc2, enc3, enc4;
			var i = 0;
			input = uTF8Encode(input);
			while (i < input.length) {
				chr1 = input.charCodeAt(i++);
				chr2 = input.charCodeAt(i++);
				chr3 = input.charCodeAt(i++);
				enc1 = chr1 >> 2;
				enc2 = ((chr1 & 3) << 4) | (chr2 >> 4);
				enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);
				enc4 = chr3 & 63;
				if (isNaN(chr2)) {
					enc3 = enc4 = 64;
				} else if (isNaN(chr3)) {
					enc4 = 64;
				}
				output = output + keyString.charAt(enc1) + keyString.charAt(enc2) + keyString.charAt(enc3) + keyString.charAt(enc4);
			}
			return output;
		},
		base64Decode: function(input) {
			var output = "";
			var chr1, chr2, chr3;
			var enc1, enc2, enc3, enc4;
			var i = 0;
			input = input.replace(/[^A-Za-z0-9\+\/\=]/g, "");
			while (i < input.length) {
				enc1 = keyString.indexOf(input.charAt(i++));
				enc2 = keyString.indexOf(input.charAt(i++));
				enc3 = keyString.indexOf(input.charAt(i++));
				enc4 = keyString.indexOf(input.charAt(i++));
				chr1 = (enc1 << 2) | (enc2 >> 4);
				chr2 = ((enc2 & 15) << 4) | (enc3 >> 2);
				chr3 = ((enc3 & 3) << 6) | enc4;
				output = output + String.fromCharCode(chr1);
				if (enc3 != 64) {
					output = output + String.fromCharCode(chr2);
				}
				if (enc4 != 64) {
					output = output + String.fromCharCode(chr3);
				}
			}
			output = uTF8Decode(output);
			return output;
		}
	});
})(jQuery);

(function($) {
    var locationWrapper = {
        put: function(hash, win) {
            (win || window).location.hash = this.encoder(hash);
        },
        get: function(win) {
            var hash = ((win || window).location.hash).replace(/^#/, '');
            try {
                return $.browser.mozilla ? hash : decodeURIComponent(hash);
            }
            catch (error) {
                return hash;
            }
        },
        encoder: encodeURIComponent
    };

    var iframeWrapper = {
        id: "__jQuery_history",
        init: function() {
            var html = '<iframe id="'+ this.id +'" style="display:none" src="javascript:false;" />';
            $("body").prepend(html);
            return this;
        },
        _document: function() {
            return $("#"+ this.id)[0].contentWindow.document;
        },
        put: function(hash) {
            var doc = this._document();
            doc.open();
            doc.close();
            locationWrapper.put(hash, doc);
        },
        get: function() {
            return locationWrapper.get(this._document());
        }
    };

    function initObjects(options) {
        options = $.extend({
                unescape: false
            }, options || {});

        locationWrapper.encoder = encoder(options.unescape);

        function encoder(unescape_) {
            if(unescape_ === true) {
                return function(hash){ return hash; };
            }
            if(typeof unescape_ == "string" &&
               (unescape_ = partialDecoder(unescape_.split("")))
               || typeof unescape_ == "function") {
                return function(hash) { return unescape_(encodeURIComponent(hash)); };
            }
            return encodeURIComponent;
        }

        function partialDecoder(chars) {
            var re = new RegExp($.map(chars, encodeURIComponent).join("|"), "ig");
            return function(enc) { return enc.replace(re, decodeURIComponent); };
        }
    }

    var implementations = {};

    implementations.base = {
        callback: undefined,
        type: undefined,

        check: function() {},
        load:  function(hash) {},
        init:  function(callback, options) {
            initObjects(options);
            self.callback = callback;
            self._options = options;
            self._init();
        },

        _init: function() {},
        _options: {}
    };

    implementations.timer = {
        _appState: undefined,
        _init: function() {
            var current_hash = locationWrapper.get();
            self._appState = current_hash;
            self.callback(current_hash);
            setInterval(self.check, 100);
        },
        check: function() {
            var current_hash = locationWrapper.get();
            if(current_hash != self._appState) {
                self._appState = current_hash;
                self.callback(current_hash);
            }
        },
        load: function(hash) {
            if(hash != self._appState) {
                locationWrapper.put(hash);
                self._appState = hash;
                self.callback(hash);
            }
        }
    };

    implementations.iframeTimer = {
        _appState: undefined,
        _init: function() {
            var current_hash = locationWrapper.get();
            self._appState = current_hash;
            iframeWrapper.init().put(current_hash);
            self.callback(current_hash);
            setInterval(self.check, 100);
        },
        check: function() {
            var iframe_hash = iframeWrapper.get(),
                location_hash = locationWrapper.get();

            if (location_hash != iframe_hash) {
                if (location_hash == self._appState) {    // user used Back or Forward button
                    self._appState = iframe_hash;
                    locationWrapper.put(iframe_hash);
                    self.callback(iframe_hash); 
                } else {                              // user loaded new bookmark
                    self._appState = location_hash;  
                    iframeWrapper.put(location_hash);
                    self.callback(location_hash);
                }
            }
        },
        load: function(hash) {
            if(hash != self._appState) {
                locationWrapper.put(hash);
                iframeWrapper.put(hash);
                self._appState = hash;
                self.callback(hash);
            }
        }
    };

    implementations.hashchangeEvent = {
        _init: function() {
            self.callback(locationWrapper.get());
            $(window).bind('hashchange', self.check);
        },
        check: function() {
            self.callback(locationWrapper.get());
        },
        load: function(hash) {
            locationWrapper.put(hash);
        }
    };

    var self = $.extend({}, implementations.base);

    if($.browser.msie && ($.browser.version < 8 || document.documentMode < 8)) {
        self.type = 'iframeTimer';
    } else if("onhashchange" in window) {
        self.type = 'hashchangeEvent';
    } else {
        self.type = 'timer';
    }

    $.extend(self, implementations[self.type]);
    $.history = self;
})(jQuery);


jQuery.extend({

    createUploadIframe: function(id, uri)
	{
			//create frame
            var frameId = 'jUploadFrame' + id;
            
            if(window.ActiveXObject) {
                var io = document.createElement('<iframe id="' + frameId + '" name="' + frameId + '" />');
                if(typeof uri== 'boolean'){
                    io.src = 'javascript:false';
                }
                else if(typeof uri== 'string'){
                    io.src = uri;
                }
            }
            else {
                var io = document.createElement('iframe');
                io.id = frameId;
                io.name = frameId;
            }
            io.style.position = 'absolute';
            io.style.top = '-1000px';
            io.style.left = '-1000px';

            document.body.appendChild(io);

            return io			
    },
    createUploadForm: function(id, fileElementId)
	{
		//create form	
		var formId = 'jUploadForm' + id;
		var fileId = 'jUploadFile' + id;
		var form = $('<form  action="" method="POST" name="' + formId + '" id="' + formId + '" enctype="multipart/form-data"></form>');	
		var oldElement = $('#' + fileElementId);
		var newElement = $(oldElement).clone();
		$(oldElement).attr('id', fileId);
		$(oldElement).before(newElement);
		$(oldElement).appendTo(form);
		//set attributes
		$(form).css('position', 'absolute');
		$(form).css('top', '-1200px');
		$(form).css('left', '-1200px');
		$(form).appendTo('body');		
		return form;
    },

    ajaxFileUpload: function(s) {
        // TODO introduce global settings, allowing the client to modify them for all requests, not only timeout		
        s = jQuery.extend({}, jQuery.ajaxSettings, s);
        var id = new Date().getTime()        
		var form = jQuery.createUploadForm(id, s.fileElementId);
		var io = jQuery.createUploadIframe(id, s.secureuri);
		var frameId = 'jUploadFrame' + id;
		var formId = 'jUploadForm' + id;		
        // Watch for a new set of requests
        if ( s.global && ! jQuery.active++ )
		{
			jQuery.event.trigger( "ajaxStart" );
		}            
        var requestDone = false;
        // Create the request object
        var xml = {}   
        if ( s.global )
            jQuery.event.trigger("ajaxSend", [xml, s]);
        // Wait for a response to come back
        var uploadCallback = function(isTimeout)
		{			
			var io = document.getElementById(frameId);
            try 
			{				
				if(io.contentWindow)
				{
					 xml.responseText = io.contentWindow.document.body?io.contentWindow.document.body.innerHTML:null;
                	 xml.responseXML = io.contentWindow.document.XMLDocument?io.contentWindow.document.XMLDocument:io.contentWindow.document;
					 
				}else if(io.contentDocument)
				{
					 xml.responseText = io.contentDocument.document.body?io.contentDocument.document.body.innerHTML:null;
                	xml.responseXML = io.contentDocument.document.XMLDocument?io.contentDocument.document.XMLDocument:io.contentDocument.document;
				}						
            }catch(e)
			{
				jQuery.handleError(s, xml, null, e);
			}
            if ( xml || isTimeout == "timeout") 
			{				
                requestDone = true;
                var status;
                try {
                    status = isTimeout != "timeout" ? "success" : "error";
                    // Make sure that the request was successful or notmodified
                    if ( status != "error" )
					{
                        // process the data (runs the xml through httpData regardless of callback)
                        var data = jQuery.uploadHttpData( xml, s.dataType );    
                        // If a local callback was specified, fire it and pass it the data
                        if ( s.success )
                            s.success( data, status );
    
                        // Fire the global callback
                        if( s.global )
                            jQuery.event.trigger( "ajaxSuccess", [xml, s] );
                    } else
                        jQuery.handleError(s, xml, status);
                } catch(e) 
				{
                    status = "error";
                    jQuery.handleError(s, xml, status, e);
                }

                // The request was completed
                if( s.global )
                    jQuery.event.trigger( "ajaxComplete", [xml, s] );

                // Handle the global AJAX counter
                if ( s.global && ! --jQuery.active )
                    jQuery.event.trigger( "ajaxStop" );

                // Process result
                if ( s.complete )
                    s.complete(xml, status);

                jQuery(io).unbind()

                setTimeout(function()
									{	try 
										{
											$(io).remove();
											$(form).remove();	
											
										} catch(e) 
										{
											jQuery.handleError(s, xml, null, e);
										}									

									}, 100)

                xml = null

            }
        }
        // Timeout checker
        if ( s.timeout > 0 ) 
		{
            setTimeout(function(){
                // Check to see if the request is still happening
                if( !requestDone ) uploadCallback( "timeout" );
            }, s.timeout);
        }
        try 
		{
           // var io = $('#' + frameId);
			var form = $('#' + formId);
			$(form).attr('action', s.url);
			$(form).attr('method', 'POST');
			$(form).attr('target', frameId);
            if(form.encoding)
			{
                form.encoding = 'multipart/form-data';				
            }
            else
			{				
                form.enctype = 'multipart/form-data';
            }			
            $(form).submit();

        } catch(e) 
		{			
            jQuery.handleError(s, xml, null, e);
        }
        if(window.attachEvent){
            document.getElementById(frameId).attachEvent('onload', uploadCallback);
        }
        else{
            document.getElementById(frameId).addEventListener('load', uploadCallback, false);
        } 		
        return {abort: function () {}};	

    },

    uploadHttpData: function( r, type ) {
        var data = !type;
        data = type == "xml" || data ? r.responseXML : r.responseText;
        // If the type is "script", eval it in global context
        if ( type == "script" )
            jQuery.globalEval( data );
        // Get the JavaScript object, if JSON is used.
        if ( type == "json" )
            eval( "data = " + data );
        // evaluate scripts within html
        if ( type == "html" )
            jQuery("<div>").html(data).evalScripts();
			//alert($('param', data).each(function(){alert($(this).attr('value'));}));
        return data;
    }
})

jQuery.extend({ 
   evalJSON: function(strJson) {
     return eval("(" + strJson + ")"); 
   } 
}); 
jQuery.extend({ 
   toJSONString: function(object) { 
     var type = typeof object; 
     if ('object' == type) { 
       if (Array == object.constructor) type = 'array'; 
       else if (RegExp == object.constructor) type = 'regexp'; 
       else type = 'object'; 
     } 
     switch (type) { 
     case 'undefined': 
     case 'unknown': 
       return; 
       break; 
     case 'function': 
     case 'boolean': 
     case 'regexp': 
       return object.toString(); 
       break; 
     case 'number': 
       return isFinite(object) ? object.toString() : 'null'; 
       break; 
     case 'string': 
       return '"' + object.replace(/(\\|\")/g, "\\$1").replace(/\n|\r|\t/g, function() { 
         var a = arguments[0]; 
         return (a == '\n') ? '\\n': (a == '\r') ? '\\r': (a == '\t') ? '\\t': "" 
       }) + '"'; 
       break; 
     case 'object': 
       if (object === null) return 'null'; 
       var results = []; 
       for (var property in object) { 
         var value = jQuery.toJSONString(object[property]); 
         if (value !== undefined) results.push(jQuery.toJSONString(property) + ':' + value); 
       } 
       return '{' + results.join(',') + '}'; 
       break; 
     case 'array': 
       var results = []; 
       for (var i = 0; i < object.length; i++) { 
         var value = jQuery.toJSONString(object[i]); 
         if (value !== undefined) results.push(value); 
       } 
       return '[' + results.join(',') + ']'; 
       break; 
     } 
   } 
});

var jsonParse = (function () {
  var number
      = '(?:-?\\b(?:0|[1-9][0-9]*)(?:\\.[0-9]+)?(?:[eE][+-]?[0-9]+)?\\b)';
  var oneChar = '(?:[^\\0-\\x08\\x0a-\\x1f\"\\\\]'
      + '|\\\\(?:[\"/\\\\bfnrt]|u[0-9A-Fa-f]{4}))';
  var string = '(?:\"' + oneChar + '*\")';

  // Will match a value in a well-formed JSON file.
  // If the input is not well-formed, may match strangely, but not in an unsafe
  // way.
  // Since this only matches value tokens, it does not match whitespace, colons,
  // or commas.
  var jsonToken = new RegExp(
      '(?:false|true|null|[\\{\\}\\[\\]]'
      + '|' + number
      + '|' + string
      + ')', 'g');

  // Matches escape sequences in a string literal
  var escapeSequence = new RegExp('\\\\(?:([^u])|u(.{4}))', 'g');

  // Decodes escape sequences in object literals
  var escapes = {
    '"': '"',
    '/': '/',
    '\\': '\\',
    'b': '\b',
    'f': '\f',
    'n': '\n',
    'r': '\r',
    't': '\t'
  };
  function unescapeOne(_, ch, hex) {
    return ch ? escapes[ch] : String.fromCharCode(parseInt(hex, 16));
  }

  // A non-falsy value that coerces to the empty string when used as a key.
  var EMPTY_STRING = new String('');
  var SLASH = '\\';

  // Constructor to use based on an open token.
  var firstTokenCtors = { '{': Object, '[': Array };

  var hop = Object.hasOwnProperty;

  return function (json, opt_reviver) {
    // Split into tokens
    var toks = json.match(jsonToken);
    // Construct the object to return
    var result;
    var tok = toks[0];
    var topLevelPrimitive = false;
    if ('{' === tok) {
      result = {};
    } else if ('[' === tok) {
      result = [];
    } else {
      // The RFC only allows arrays or objects at the top level, but the JSON.parse
      // defined by the EcmaScript 5 draft does allow strings, booleans, numbers, and null
      // at the top level.
      result = [];
      topLevelPrimitive = true;
    }

    // If undefined, the key in an object key/value record to use for the next
    // value parsed.
    var key;
    // Loop over remaining tokens maintaining a stack of uncompleted objects and
    // arrays.
    var stack = [result];
    for (var i = 1 - topLevelPrimitive, n = toks.length; i < n; ++i) {
      tok = toks[i];

      var cont;
      switch (tok.charCodeAt(0)) {
        default:  // sign or digit
          cont = stack[0];
          cont[key || cont.length] = +(tok);
          key = void 0;
          break;
        case 0x22:  // '"'
          tok = tok.substring(1, tok.length - 1);
          if (tok.indexOf(SLASH) !== -1) {
            tok = tok.replace(escapeSequence, unescapeOne);
          }
          cont = stack[0];
          if (!key) {
            if (cont instanceof Array) {
              key = cont.length;
            } else {
              key = tok || EMPTY_STRING;  // Use as key for next value seen.
              break;
            }
          }
          cont[key] = tok;
          key = void 0;
          break;
        case 0x5b:  // '['
          cont = stack[0];
          stack.unshift(cont[key || cont.length] = []);
          key = void 0;
          break;
        case 0x5d:  // ']'
          stack.shift();
          break;
        case 0x66:  // 'f'
          cont = stack[0];
          cont[key || cont.length] = false;
          key = void 0;
          break;
        case 0x6e:  // 'n'
          cont = stack[0];
          cont[key || cont.length] = null;
          key = void 0;
          break;
        case 0x74:  // 't'
          cont = stack[0];
          cont[key || cont.length] = true;
          key = void 0;

          break;
        case 0x7b:  // '{'
          cont = stack[0];
          stack.unshift(cont[key || cont.length] = {});
          key = void 0;
          break;
        case 0x7d:  // '}'
          stack.shift();
          break;
      }
    }
    // Fail if we've got an uncompleted object.
    if (topLevelPrimitive) {
      if (stack.length !== 1) { throw new Error(); }
      result = result[0];
    } else {
      if (stack.length) { throw new Error(); }
    }

    if (opt_reviver) {
      // Based on walk as implemented in http://www.json.org/json2.js
      var walk = function (holder, key) {
        var value = holder[key];
        if (value && typeof value === 'object') {
          var toDelete = null;
          for (var k in value) {
            if (hop.call(value, k) && value !== holder) {
              // Recurse to properties first.  This has the effect of causing
              // the reviver to be called on the object graph depth-first.

              // Since 'this' is bound to the holder of the property, the
              // reviver can access sibling properties of k including ones
              // that have not yet been revived.

              // The value returned by the reviver is used in place of the
              // current value of property k.
              // If it returns undefined then the property is deleted.
              var v = walk(value, k);
              if (v !== void 0) {
                value[k] = v;
              } else {
                // Deleting properties inside the loop has vaguely defined
                // semantics in ES3 and ES3.1.
                if (!toDelete) { toDelete = []; }
                toDelete.push(k);
              }
            }
          }
          if (toDelete) {
            for (var i = toDelete.length; --i >= 0;) {
              delete value[toDelete[i]];
            }
          }
        }
        return opt_reviver.call(holder, key, value);
      };
      result = walk({ '': result }, '');
    }

    return result;
  };
})();
	
(function ($) {
	
    $.fn.imgtool = function () {
        var maxWidth;
        $(this).live('click', function () {
            var $this = $(this),
				maxImg = $this.attr('href'),
                viewImg = $this.attr('rel').length === 0 ? maxImg : $this.attr('rel'); // 如果连接含有rel属性，则新窗口打开的原图地址为此rel里面的地址
            if ($this.find('.loading').length == 0) $this.append('<span class="loading">加载中..</span>');
            imgTool($this, maxImg, viewImg);
            return false;
        });

        // 图片预先加载
        var loadImg = function (url, fn) {
            var img = new Image();
            img.src = url;
            if (img.complete) {
                fn.call(img);
            } else {
                img.onload = function () {
                    fn.call(img);
                };
            };
        };

        // 图片工具条
        var imgTool = function (on, maxImg, viewImg) {
            var width = 0, height = 0, maxWidth = 500;//on.parent().innerWidth(),
                tool = function () {
                    on.find('.loading').remove();
                    on.hide();

                    if (on.next('.imgtoolbox').length != 0) {
                        return on.next('.imgtoolbox').show();
                    };

                    var raw_height = height,
                        raw_width = width;
				
                    if (width > maxWidth) {
                        height = maxWidth / width * height;
                        width = maxWidth;
                    };

                    var html = '<div class="imgtoolbox"><div class="tool"><a class="hideImg" href="javascript:;" title="\u6536\u8D77">\u6536\u8D77</a><a class="imgLeft" href="javascript:;" title="\u5411\u53F3\u8F6C">\u5411\u53F3\u8F6C</a><a class="imgRight" href="javascript:;" title="\u5411\u5DE6\u8F6C">\u5411\u5DE6\u8F6C</a><a class="viewImg" href="' + viewImg + '" title="\u67E5\u770B\u539F\u56FE">\u67E5\u770B\u539F\u56FE</a></div><a href="' + viewImg + '" class="maxImgLink"> <img class="maxImg" width="' + width + '" height="' + height + '" maxWidth="' + maxWidth + '" raw_width="' + raw_width + '" raw_height="' + raw_height + '" src="' + maxImg + '" /></a></div>';
                    on.after(html);
                    var box = on.next('.imgtoolbox');
                    box.hover(function () {
                        box.addClass('js_hover');
                    }, function () {
                        box.removeClass('js_hover');
                    });
                    box.find('a').bind('click', function () {
						var $this = $(this);
                        // 收起
                        if ($this.hasClass('hideImg') || $this.hasClass('maxImgLink')) {
                            box.hide();
                            box.prev().show();
                        };
                        // 左旋转
                        if ($this.hasClass('imgLeft')) {
                            box.find('.maxImg').rotate('left', maxWidth)
                        };
                        // 右旋转
                        if ($this.hasClass('imgRight')) {
                            box.find('.maxImg').rotate('right', maxWidth)
                        };
                        // 新窗口打开
                        if ($this.hasClass('viewImg')) window.open(viewImg);

                        return false;
                    });

                };

            loadImg(maxImg, function () {
                width = this.width;
                height = this.height;
                tool();
            });

        };
    };
	

	// 图片旋转
	$.fn.rotate = function (name, maxWidth) {

		var img = $(this)[0],
			step = img.getAttribute('step');

		if (!this.data('width') && !$(this).data('height')) {
			this.data('width', img.width);
			this.data('height', img.height);
		};

		if (step == null) step = 0;
		if (name === 'left') {
			(step == 3) ? step = 0 : step++;
		} else if (name === 'right') {
			(step == 0) ? step = 3 : step--;
		};
		img.setAttribute('step', step);
		var show_width = this.data('width'),
			show_height = this.data('height');
		if ((step == 1 || step == 3) && this.data('width') < this.data('height') && this.data('height') > maxWidth) {
			show_height = maxWidth;
			show_width = this.data('width') * maxWidth / this.data('height');
		}
		// IE浏览器使用滤镜旋转
		if (document.all) {
			img.style.filter = 'progid:DXImageTransform.Microsoft.BasicImage(rotation=' + step + ')';
			img.width = show_width;
			img.height = show_height;
			// IE8高度设置
			if ($.browser.version == 8) {
				switch (step) {
				case 0:
					this.parent().height('');
					break;
				case 1:
					this.parent().height(this.data('width') + 10);
					break;
				case 2:
					this.parent().height('');
					break;
				case 3:
					this.parent().height(this.data('width') + 10);
					break;
				};
			};
			// 对现代浏览器写入HTML5的元素进行旋转： canvas
		} else {
			var c = this.next('canvas')[0];
			if (this.next('canvas').length == 0) {
				this.css({
					'visibility': 'hidden',
					'position': 'absolute'
				});
				c = document.createElement('canvas');
				c.setAttribute('class', 'maxImg canvas');
				img.parentNode.appendChild(c);
			}
			var canvasContext = c.getContext('2d');
			var resizefactor = 1;
			show_height = img.raw_height = $(img).attr('raw_height');	//图片原始高度
			show_width = img.raw_width = $(img).attr('raw_width'); 		//原始宽度
			if ((step == 1 || step == 3) && img.raw_height > maxWidth) {
				resizefactor = maxWidth / img.raw_height;
				show_height = maxWidth;
				show_width = resizefactor * img.raw_width;
			}
			if ((step == 0 || step == 2) && img.raw_width > maxWidth) {
				resizefactor = maxWidth / img.raw_width;
				show_height = resizefactor * img.raw_height;
				show_width = maxWidth;
			}
			switch (step) {
			default:
			case 0:
				c.setAttribute('width', show_width);
				c.setAttribute('height', show_height);
				canvasContext.rotate(0 * Math.PI / 180);
				canvasContext.scale(resizefactor, resizefactor);						
				canvasContext.drawImage(img, 0, 0);
				break;
			case 1:
				c.setAttribute('width', show_height);
				c.setAttribute('height', show_width);
				canvasContext.rotate(90 * Math.PI / 180);
				canvasContext.scale(resizefactor, resizefactor);
				canvasContext.drawImage(img, 0, -img.raw_height);
				break;
			case 2:
				c.setAttribute('width', show_width);
				c.setAttribute('height', show_height);
				canvasContext.rotate(180 * Math.PI / 180);
				canvasContext.scale(resizefactor, resizefactor);
				canvasContext.drawImage(img, -img.raw_width, -img.raw_height);
				break;
			case 3:
				c.setAttribute('width', show_height);
				c.setAttribute('height', show_width);
				canvasContext.rotate(270 * Math.PI / 180);
				canvasContext.scale(resizefactor, resizefactor);
				canvasContext.drawImage(img, -img.raw_width, 0);
				break;
			};
		};
	};


})(jQuery);