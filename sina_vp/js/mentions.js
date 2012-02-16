Drupal.behaviors.mention = function (context) {
	
	$(function() {
		var availableTags = [
			{ label: "ActionScript", category: "" },
			{ label: "AppleScript", category: "" },
			{ label: "antal", category: "" },
			{ label: "annhhx10", category: "Products" },
			{ label: "annk K12", category: "Products" },
			{ label: "annttop C13", category: "Products" },
			{ label: "anders andersson", category: "People" },
			{ label: "andreas andersson", category: "People" },
			{ label: "andreas johnson", category: "People" }			
		];
		function split( val ) {
			return val.split( /,\s*/ );
		}
		function extractLast( term ) {
			return split( term ).pop();
		}
		$( "#edit-title" )
			// don't navigate away from the field on tab when selecting an item
			.bind( "keydown", function( event ) {
					if ( event.keyCode === $.ui.keyCode.TAB &&
						$( this ).data( "autocomplete" ).menu.active ) {
					event.preventDefault();
				}
			})
			         
			.autocomplete({
				minLength: 2,
				source: function( request, response ) {
					// delegate back to autocomplete, but extract the last term
					//response( $.ui.autocomplete.filter(availableTags, extractLast( request.term ) ) );
					$.ajax({
						url: "http://dev.weipujie.com/?q=getMyfollows",
						dataType: "json",
						type: 'POST',
						success: function( data ) {
								 if( request.term.search(/,@\w+/) != -1 )
								  { 
									response( $.ui.autocomplete.filter(
									  data, extractLast( request.term ).replace(/@/, "") ) );
								  }else  if( request.term.search(/^@/) != -1 )
								  {
									response( $.ui.autocomplete.filter(
									  data, extractLast( request.term.replace(/^@(\w+)/, "$1") ) ) );
								  }
								  console.log('data',data);
						},
						error: function(XMLHttpRequest, textStatus, errorThrown){
								alert('发生错误，请联系管理员20111214');
								 console.log('textStatus',textStatus);
								 console.log('XMLHttpRequest',XMLHttpRequest);
								 console.log('errorThrown',errorThrown);
								}
					}); 
					  
				},
				focus: function() {
					// prevent value inserted on focus
					return false;
				},
				search: function(event, ui) {
					//var terms = split( this.value ); 					
					//var term = extractLast(terms);
					console.log('terms',this.value);
				},
				select: function( event, ui ) {
					var terms = split( this.value );
					// remove the current input
					terms.pop();
					// add the selected item
					terms.push( "@"+ui.item.value );
					// add placeholder to get the comma-and-space at the end
					terms.push( "" );
					//terms.unshift( "@" );
					this.value = terms.join( "," );
					return false;
				}
			});
	});
}