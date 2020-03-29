'use strict'; if (!HTMLElement.prototype.addClass) {

  function createElement(tag, content, style) {
    let el = document.createElement(tag)
    if (content) el.innerHTML = content
    // this.appendChild(r)
    if (style) {
      if (style.constructor == CSSStyleDeclaration)
        el.style = style
      else {
        let s = json(style).slice(1, -1)
        s = s.replace(/,/g, ';').replace(/\"/gm, '')
        el.style = s
      }
    }
    return el
  }

  HTMLElement.prototype.add = function (tag, content, style) {
    if (!content) {
      let el = createElement(tag, content, style);
      this.appendChild(el)
      return el
    }

    if (typeof content == 'string') content = [content]
    if (!Array.isArray(content)) content = [content.toString()]

    let res = []
    content.forEach(v => {
      let el = createElement(tag, v, style)
      // let el = document.createElement(tag);
      // el.innerHTML = v
      this.appendChild(el)
      res.push(el)
    })
    if (res.length == 1) return res[0]
    else {
      let res = this.get('*') // let nl = NodeList(res)
      return res
    }
  }//; body.add('p', 'Hello').addClass('red')

  HTMLElement.prototype.addClass = function (classes) {
    if (this['classList']) this.classList.add(classes)
    else if (this.className.split(/\s+/).indexOf(classes) >= 0) this.className += " " + name;
    return this
  }

  window.log = console.log;
  window.json = JSON.stringify
  window.html = document.getElementsByTagName('html')[0]

  let els = document.getElementsByTagName('head')
  window.head = els.length ? els[0] : html.add('head')

  els = document.getElementsByTagName('body')
  window.body = els.length ? els[0] : html.add('body')

  window.add = (tag, content, style) => body.add(tag, content, style)

  window.include = function (src, callback) {
    var script = html.add('script')
    script.type = "text/javascript"
    script.defer = true;
    if (callback) { script.onload = callback; script.src = src; return script }
    else return new Promise(function (resolve, reject) {
      script.onload = resolve
      script.onerror = function (ev) { reject('include(): File not found.\n' + ev.target.src) }
      script.src = src
    })
  }

  window.beep = () => {
    var snd = 'data:audio/wav;base64,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qJrx4KOQjahZxWbcZgztj2c49nKmkId44S71j0c8eV9yDK6uPRzx5X18eDvjvQ6yKo9ZSS6l//8elePK/Lf//IInrOF/FvDoADYAGBMGb7FtErm5MXMlmPAJQVgWta7Zx2go+8xJ0UiCb8LHHdftWyLJE0QIAIsI+UbXu67dZMjmgDGCGl1H+vpF4NSDckSIkk7Vd+sxEhBQMRU8j/12UIRhzSaUdQ+rQU5kGeFxm+hb1oh6pWWmv3uvmReDl0UnvtapVaIzo1jZbf/pD6ElLqSX+rUmOQNpJFa/r+sa4e/pBlAABoAAAAA3CUgShLdGIxsY7AUABPRrgCABdDuQ5GC7DqPQCgbbJUAoRSUj+NIEig0YfyWUho1VBBBA//uQZB4ABZx5zfMakeAAAAmwAAAAF5F3P0w9GtAAACfAAAAAwLhMDmAYWMgVEG1U0FIGCBgXBXAtfMH10000EEEEEECUBYln03TTTdNBDZopopYvrTTdNa325mImNg3TTPV9q3pmY0xoO6bv3r00y+IDGid/9aaaZTGMuj9mpu9Mpio1dXrr5HERTZSmqU36A3CumzN/9Robv/Xx4v9ijkSRSNLQhAWumap82WRSBUqXStV/YcS+XVLnSS+WLDroqArFkMEsAS+eWmrUzrO0oEmE40RlMZ5+ODIkAyKAGUwZ3mVKmcamcJnMW26MRPgUw6j+LkhyHGVGYjSUUKNpuJUQoOIAyDvEyG8S5yfK6dhZc0Tx1KI/gviKL6qvvFs1+bWtaz58uUNnryq6kt5RzOCkPWlVqVX2a/EEBUdU1KrXLf40GoiiFXK///qpoiDXrOgqDR38JB0bw7SoL+ZB9o1RCkQjQ2CBYZKd/+VJxZRRZlqSkKiws0WFxUyCwsKiMy7hUVFhIaCrNQsKkTIsLivwKKigsj8XYlwt/WKi2N4d//uQRCSAAjURNIHpMZBGYiaQPSYyAAABLAAAAAAAACWAAAAApUF/Mg+0aohSIRobBAsMlO//Kk4soosy1JSFRYWaLC4qZBYWFRGZdwqKiwkNBVmoWFSJkWFxX4FFRQWR+LsS4W/rFRb/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////VEFHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAU291bmRib3kuZGUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAMjAwNGh0dHA6Ly93d3cuc291bmRib3kuZGUAAAAAAAAAACU='
    new Audio(snd).play()
  }

  /* DOM Handling  /*/

  // if (typeof require != 'undefined')
  //   module.exports = { get, set, on, put, select, beep, Template } // todo: off, add, remove

  // window.Template = function (key) {
  //   let tmp = document.createElement('template')
  //   tmp.innerHTML = document.currentScript.ownerDocument.querySelector(key).innerHTML
  //   return tmp
  // }

  window.find = function (key) {
    if (key == 'window') return [window]
    var r
    try {
      key = key.trim()
      switch (key.charAt(0)) {
        case '#': r = [document.getElementById(key.substr(1))]; break
        case '.': r = [...document.getElementsByClassName(key.substr(1))]; break
        default: r = [...document.getElementsByTagName(key)]
      }
    } catch (err) {
      r = [document.getElementById(key)]
      if (!r) r = [...document.getElementsByClassName(key)]
      if (!r) throw new Error(`get() Element ${key} not found.`)
    }
    return r
  }

  window.select = function (key) {
    return [...document.querySelectorAll(key)]
  }

  window.get = function (key) {
    let r = find(key)
    return (r.length == 1) ? r[0] : r
  }

  window.set = function (keys, value = '') {
    keys.split(',').map(key => {
      try {
        const [, prefix, tag, attribute] = /([^\w]?)([^\s\.]+)\.?([\w-_]+)?/.exec(key.trim())
        find(prefix + tag).map((e) => {
          if (attribute == 'value')
            e.value = value
          else if (attribute)
            e.setAttribute(attribute, value)
          else //if (isHtml(value))
            e.innerHTML = value
          // else e.textContent = value
        })
      } catch (error) {
        throw new Error(`set(${key}): Not found`)
      }
    })
  }

  // on('#title.click, .menu.click, nav.click', { alert('Hello') })
  window.on = function (keys, callback) {
    keys.split(/ | *,\ */).map((key) => {
      try {
        const [, prefix, tag, event] = /([\.#]?)([\w\d-]+)\.([\w\d]+)/.exec(key)
        find(prefix + tag)
          .map((e) =>
            e.addEventListener(event, callback))
      } catch (error) {
        throw new Error(`on(${keys}): Not found`)
      }
    })
  }

  window.put = function (html) {
    // https://developer.mozilla.org/en-US/docs/Web/API/Element/insertAdjacentHTML
    document.innerHTML += `<div>${html}</div>`
  }

  // from bling.js ====================================================

  if (!window.$) window.$ = get
  if (!window.$$) window.$$ = select

  Node.prototype.on = window.on = function (events, fn) {
    events.split(/ | *, */).map((ev) => this.addEventListener(ev, fn))
    return this
  }

  Element.prototype.get = function (key) {
    return this.querySelectorAll(key)
  }

  Object.setPrototypeOf(NodeList.prototype, Array.prototype)

  NodeList.prototype.on = NodeList.prototype.addEventListener = function (name, fn) {
    this.forEach((elem, i) => elem.on(name, fn))
    return this
  }

  // add.elements shortcut ====================================================
  add.p = (text, style) => add('p', text, style)
  add.h1 = (text, style) => add('h1', text, style)

  add.a = (href, text) => {
    let res = add('a', text || href)
    res.href = href
    return res
  }

  add.button = (text, onclick) => {
    let r = add('button', text)
    r.addEventListener('click', onclick)
    return r
  }

  add.html = function () {
    // <a href="#" onclick="chrome.tabs.create({url:'chrome://settings/clearBrowserData'});">click me</a>
  }
}