// Create the tooltips only when document ready
$(document).ready(function()
        {
        $('a').each(function() {
            $(this).qtip({
content: {
text: function(event, api) {
$.ajax({
url: api.elements.target.attr('href') // Use href attribute as URL
})
.then(function(content) {
    // Set the tooltip content upon successful retrieval
    api.set('content.text', content);
    }, function(xhr, status, error) {
    // Upon failure... set the tooltip content to error
    api.set('content.text', status + ': ' + error);
    });

return 'Loading...'; // Set some initial text
}
},
position: {
    target: 'mouse'
},
style: {
    classes: 'qtip-bootstrap'
}
            });
});
});
