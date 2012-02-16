// $Id: sina_open.js,v 1.2 2011/02/11 03:52:44 eastcn Exp $
Drupal.behaviors.select_all = function () {	
		//一键关注：
		$('.focus_recommend').click(function(){
	  			dialog("一键关注","id:sina_vp_recommend","600px","auto","id");  	
	  });
	  //一键关注 全选/取消     
    $(".form-checkbox").attr("checked",'true');//全选
    console.log($(".select-all4vp"));
  	if(!$(this).attr("checked"))
	   {
	   $(".form-checkbox").removeAttr("checked");
	    console.log(2);
	   }
     $(".select-all4vp").click(function(){
     	 console.log(3);
     });

}
