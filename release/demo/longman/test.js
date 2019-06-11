window.onload = function() 
{
    console.log("onload");
            $('a').click(function()
            {
				console.log($(this).attr('href'));
				return false
			});
			$('span').dblclick(function()
            {
				var txt = window.getSelection().toString();//获取选中内容
				console.log(txt);
			});
                
 }