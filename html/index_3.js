      var canvas = document.getElementById('myCanvas');       var context = canvas.getContext('2d');       var centerX = canvas.width / 2;       var centerY = canvas.height / 2;       var radius = 10;        context.beginPath();       context.arc(centerX, centerY, radius, 0, 2 * Math.PI, false);       context.fillStyle = %light_status%;       context.fill();       context.lineWidth = 2;       context.strokeStyle = '#999999';       context.stroke();  