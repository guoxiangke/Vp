if (Drupal.jsEnabled) {
  var province_city_select = function(pid, cid, aid)
  {
    var province_city_form = $('#'+pid).parents('form');
    $('#'+pid).change(function(){
      var pvalue = $(this).val();
      var options = {
        url: Drupal.settings.basePath+'province_city/js/city/'+pvalue,
        beforeSend: function()
        {
          //console.log(Drupal.settings.basePath+'province_city/js/city/'+pvalue);
          $('#'+cid).attr('disabled','disabled');
        },
        success: function (matches) {
          $('#'+cid).attr('disabled','').empty();
          $.each(matches, function(v,o){
            $('<option value="'+v+'">'+o+'</option>').appendTo($('#'+cid));
          });
          if( typeof aid != 'undefined' )
          {
            $('#'+cid).change(function(){
              var cvalue = $(this).val();
              var options2 = {
                url: Drupal.settings.basePath+'province_city/js/area/'+cvalue,
                beforeSend: function()
                {
                  $('#'+aid).attr('disabled','disabled');
                },
                success: function (matches) {
                  $('#'+aid).attr('disabled','').empty();
                  $.each(matches, function(v,o){
                    $('<option value="'+v+'">'+o+'</option>').appendTo($('#'+aid));
                  });
                  //$('#'+cid).attr('disable','');
                },
                error: function (xmlhttp) {
                  //console.log(Drupal.ahahError(xmlhttp, Drupal.settings.basePath+'province_city/js/area/'+cvalue));
                },
                dataType: 'json',
                type: 'POST'
              };
              province_city_form.ajaxSubmit(options2);
            });
          }
          //$('#'+cid).attr('disable','');
        },
        error: function (xmlhttp) {
          //console.log(Drupal.ahahError(xmlhttp, Drupal.settings.basePath+'province_city/js/city/'+pvalue));
        },
        dataType: 'json',
        type: 'POST'
      };
      province_city_form.ajaxSubmit(options);
    });
  }
  
  Drupal.theme.prototype.pc_select = function (title, options, id, name) {
    var select_output = '<select name="'+name+'" id="'+id+'"></select>';
    return select_output;
  };
  
  
  Drupal.behaviors.pc_select = function(context)
  {
    $.each( Drupal.settings.province_city, function(k, v){
      switch( v.province_city_depth )
      {
        case '2':
          province_city_select(v.province_id, v.city_id);
          break;
        case '3':
          province_city_select(v.province_id, v.city_id, v.area_id);
          break;
      }
    });
  }
}