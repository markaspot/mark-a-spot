/* Load this script using conditional IE comments if you need to support IE 7 and IE 6. */

window.onload = function() {
	function addIcon(el, entity) {
		var html = el.innerHTML;
		el.innerHTML = '<span style="font-family: \'markaspot\'">' + entity + '</span>' + html;
	}
	var icons = {
			'icon-brightness-medium' : '&#xe000;',
			'icon-remove' : '&#xe001;',
			'icon-bullhorn' : '&#xe002;',
			'icon-droplet' : '&#xe003;',
			'icon-location' : '&#xe004;',
			'icon-location-2' : '&#xe005;',
			'icon-office' : '&#xe006;',
			'icon-bubbles' : '&#xe007;',
			'icon-lock' : '&#xe008;',
			'icon-bug' : '&#xe009;',
			'icon-leaf' : '&#xe00a;',
			'icon-lab' : '&#xe00c;',
			'icon-lightbulb' : '&#xf0eb;',
			'icon-bubbles-2' : '&#xe00f;',
			'icon-checkmark' : '&#xe010;',
			'icon-play' : '&#xe011;',
			'icon-close' : '&#xe012;',
			'icon-pause' : '&#xe00e;',
			'icon-headphones' : '&#xf025;',
			'icon-thumbs-up' : '&#xf164;',
			'icon-thumbs-down' : '&#xf165;',
			'icon-headphones-2' : '&#xe013;',
			'icon-drawer' : '&#xe014;',
			'icon-car' : '&#xe00d;',
			'icon-trash' : '&#xf014;',
			'icon-eye-open' : '&#xf06e;',
			'icon-fighter-jet' : '&#xf0fb;',
			'icon-graffiti' : '&#xf104;'
		},
		els = document.getElementsByTagName('*'),
		i, attr, html, c, el;
	for (i = 0; ; i += 1) {
		el = els[i];
		if(!el) {
			break;
		}
		attr = el.getAttribute('data-icon');
		if (attr) {
			addIcon(el, attr);
		}
		c = el.className;
		c = c.match(/icon-[^\s'"]+/);
		if (c && icons[c[0]]) {
			addIcon(el, icons[c[0]]);
		}
	}
};