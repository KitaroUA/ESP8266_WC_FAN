var formatNum = function (num) 
{
if (num.toString().length < 2) {
return '0' + num;
 }
return num;
};
var option_1 = function (o_selected,elementId){
var min = 0;
var max = 1425;
select = document.getElementById(elementId);
for (var i = min; i<=max; i+=15){
var opt = document.createElement('option');
opt.value = i;
opt.innerHTML = formatNum(Math.trunc(i/60))+":"+formatNum(i%60);
if (i == o_selected) { opt.selected = true; } 
select.appendChild(opt); 
}
}
//Some code to protect our world