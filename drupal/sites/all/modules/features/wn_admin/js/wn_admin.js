// See http://www.packtpub.com/article/jquery-table-manipulation-part1

$(document).ready(function() {
  $('table.sortable').each(function() {
    var $table = $(this);
    $('th', $table).each(function(column) {
      if ($(this).is('.sort-alpha')) {
        $(this).addClass('clickable').hover(function() {
          $(this).addClass('hover');
        }, function() {
          $(this).removeClass('hover');
        }).click(function() {
          if ($(this).is('.sorted-asc')) {
            var rows = $table.find('tbody > tr').get();
            rows.sort(function(a, b) {
              var keyA = $(a).children('td').eq(column).text()
                                                        .toUpperCase();
              var keyB = $(b).children('td').eq(column).text()
                                                        .toUpperCase();
              if (keyA > keyB) return -1;
              if (keyA < keyB) return 1;
              return 0;
            });
            $.each(rows, function(index, row) {
              $table.children('tbody').append(row);
            });
            $('th').each(function() {
              $(this).removeClass('sorted-asc');
              $(this).removeClass('sorted-desc');
            })
            $(this).addClass('sorted-desc');
          }
          else
          {
            var rows = $table.find('tbody > tr').get();
            rows.sort(function(a, b) {
              var keyA = $(a).children('td').eq(column).text()
                                                        .toUpperCase();
              var keyB = $(b).children('td').eq(column).text()
                                                        .toUpperCase();
              if (keyA < keyB) return -1;
              if (keyA > keyB) return 1;
              return 0;
            });
            $.each(rows, function(index, row) {
              $table.children('tbody').append(row);
            });
            $('th').each(function() {
              $(this).removeClass('sorted-asc');
              $(this).removeClass('sorted-desc');
            })
            $(this).addClass('sorted-asc');
          }
        });
      }
    });
  });
});