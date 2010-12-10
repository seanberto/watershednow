﻿/*
Copyright (c) 2003-2010, CKSource - Frederico Knabben. All rights reserved.
For licensing, see LICENSE.html or http://ckeditor.com/license
*/

/**
 * @file AutoGrow plugin
 */
(function(){
	var resizeEditor = function( editor )
	{
		var doc = editor.document,
			currentHeight = editor.window.getViewPaneSize().height,
			newHeight;

		// We can not use documentElement to calculate the height for IE (#6061).
		if ( CKEDITOR.env.ie )
			newHeight = doc.getBody().$.scrollHeight + 24;
		else
			newHeight = doc.getDocumentElement().$.offsetHeight;

		var min = editor.config.autoGrow_minHeight,
			max = editor.config.autoGrow_maxHeight;
		( min == undefined ) && ( editor.config.autoGrow_minHeight = min = 200 );
		if ( min )
			newHeight = Math.max( newHeight, min );
		if ( max )
			newHeight = Math.min( newHeight, max );

		if ( newHeight != currentHeight )
		{
			newHeight = editor.fire( 'autoGrow', { currentHeight : currentHeight, newHeight : newHeight } ).newHeight;
			editor.resize( editor.container.getStyle( 'width' ), newHeight, true );
		}
	};
	CKEDITOR.plugins.add( 'autogrow',
	{
		init : function( editor )
		{
			for ( var eventName in { contentDom:1, key:1, selectionChange:1, insertElement:1 } )
			{
				editor.on( eventName, function( evt )
				{
					// Some time is required for insertHtml, and it gives other events better performance as well.
					if ( evt.editor.mode == 'wysiwyg' )
						setTimeout( function(){ resizeEditor( evt.editor ); }, 100 );
				});
			}
		}
	});
})();
/**
 * The minimum height to which the editor can reach using AutoGrow.
 * @name CKEDITOR.config.autoGrow_minHeight
 * @type Number
 * @default 200
 * @since 3.4
 * @example
 * config.autoGrow_minHeight = 300;
 */

/**
 * The maximum height to which the editor can reach using AutoGrow. Zero means unlimited.
 * @name CKEDITOR.config.autoGrow_maxHeight
 * @type Number
 * @default 0
 * @since 3.4
 * @example
 * config.autoGrow_maxHeight = 400;
 */

/**
 * Fired when the AutoGrow plugin is about to change the size of the editor.
 * @name CKEDITOR#autogrow
 * @event
 * @param {Number} data.currentHeight The current height of the editor (before the resizing).
 * @param {Number} data.newHeight The new height of the editor (after the resizing). It can be changed
 *				to determine another height to be used instead.
 */
