# Custom theme für Sag's uns 2

## Info
Dieses theme basiert auf mas_olivero, das grob auf dem 9.x Basis-Theme Olivero basiert. Sämtliche Anpassungen an der Plattform
sollten in diesem Custom Theme vorgenommen werden. 

## Entwicklung

1. Yarn installieren, falls noch nicht global verfügbar:

```
$ cd
$ npm install yarn
```

2. Dev-Libraries installieren

```
$ cd /web/themes/custom/mas_custom
$ npm install
$ yarn watch
```

3. CSS Anpassen / JS Entwickeln mit ES6

CSS Files werden via postcss bearbeitet und via yarn watch beim Speichern in CSS umgewandelt, JS wird in ECMAScript 2015+ (ES6+) entwickelt und automatisch nach IE11 kompatibles JS kompiliert.

