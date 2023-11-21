var touchRegionElement = document.getElementById('touch-region');
var outputElement = document.getElementById('output');

touchRegionElement.addEventListener('touchstart', function(e) {
   outputElement.innerText = 'Touch begins';
});

touchRegionElement.addEventListener('touchend', function(e) {
   outputElement.innerText = 'Touch ends';
});


function createCircle(backgroundColor) {
   var circleElement = document.createElement('div');
   circleElement.className = 'circle';
   circleElement.style.backgroundColor = backgroundColor;

   outputElement.appendChild(circleElement);
}



var touchRegionElement = document.getElementById('touch-region');
var outputElement = document.getElementById('output');

function createCircle(backgroundColor) { /* ... */ }

touchRegionElement.addEventListener('touchstart', function(e) {
   createCircle('green');
});

touchRegionElement.addEventListener('touchmove', function(e) {
   createCircle('yellow');
});

touchRegionElement.addEventListener('touchend', function(e) {
   createCircle('red');
});