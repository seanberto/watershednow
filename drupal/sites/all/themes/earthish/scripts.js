// $Id: scripts.js,v 1.1 2010/03/03 06:56:07 ishmaelsanchez Exp $ 
// Add custom JS below. Note code this will be added to all pages on the site.

if (Drupal.jsEnabled) {
	$(document).ready(function() {
	
		$('#footer-primary .content:not(:first)').hide();
		$('#footer-secondary .content:not(:first)').hide();
		$('#footer-primary h3').click(blockToggle).css('cursor', 'pointer');
		$('#footer-secondary h3').click(blockToggle).css('cursor', 'pointer');
		
		function blockToggle() {
		  
		 $(this).siblings('div.content').slideToggle();
	
		}
	});
}

/*if (Drupal.jsEnabled) {
	$(document).ready(function() {
    var $el, leftPos,
        $mainNav = $("#highlighter");
    $mainNav.append("<li id='nav-line'></li>");
    var $navLine = $("#nav-line");

    $navLine
        .css("left", $(".active a").position().left)
        .data("origLeft", $navLine.position().left)

    $("#highlighter li a").hover(function() {
        $el = $(this);
        leftPos = $el.position().left;
        $navLine.stop().animate({
            left: leftPos
        });
    }, function() {
        $navLine.stop().animate({
            left: $navLine.data("origLeft")
        });
    });
})};*/