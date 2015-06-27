/**
 *@author: Josh Snider
 */

import util.parsing.combinator._

abstract class Param {
  def toString(cls: Class) : String
  def nm(cls: String) : String = cls + "_" + nm
  val nm : String
}
case class Typed(ty: String, nm: String) extends Param {
  override def toString() = ty + " " + nm
  def toString(cls: Class) : String = ty + " " + nm(cls.header)
}

case class Method(ty: Typed, args: List[Typed], src: String) extends Param { 

  val nm = ty.nm
  override def toString() = ty + "(" + args.mkString(",") + ")" + src
  def argsString(cls: Class) : String = "(" + (List(Typed(cls.ref, "self")) ++ args).mkString(",") + ")"
  def toString(cls: Class) : String = ty.toString(cls) + argsString(cls) + src
  def prototype(cls: Class) : String = ty.toString(cls) + argsString(cls) + ";"
  def signature(cls: Class) : String = {
    ty.ty + "(*" + nm + ") " + argsString(cls)
  }
}

abstract class TopLevel

case class Include(include: String) extends TopLevel {
  override def toString() = include
}

case class Class(name: String, pubs: List[Param], priv: List[Param]) extends TopLevel {

  val header = "_" + name + "_h"
  val privmeths = priv.filter(_.isInstanceOf[Method]).map(_.asInstanceOf[Method])
  val pubmeths = pubs.filter(_.isInstanceOf[Method]).map(_.asInstanceOf[Method])
  val ref = name + "Ref"

  override def toString() = {
    "#ifndef " + header + "\n" +
      "#define " + header + "\n" +
      "//Class file for " + name + "\n" +
      "//Produced by https://github.com/vermiculus/c-util\n" +
      classStruct +
      prototypes +
      staticStruct +
      allocs() +
      methods() +
      "#endif //" + header + "\n"
  }

  def allocs() = {
    ref + "\n" +
      header + "_alloc() {\n" +
      "return (" + ref + ") calloc(sizeof(" + ref + "), 1);\n" +
      "}\n\n" +
      "void\n" +
      header + "_dealloc(" + ref + " self) {\n" +
      "free(self);\n" +
      "}\n"
  }

  def classStruct : String = {
    "typedef struct _" + ref + " {\n" +
      pubs.filter(_.isInstanceOf[Typed]).mkString(";\n") + ";\n" +
      "} *" + ref + ";\n" +
      "struct _" + ref + "_meth {\n" +
      //TODO Proper struct thing
      ref +" (*alloc) ();\n" +
      "void (*dealloc) ();\n" +
      pubmeths.map(_.signature(this)).mkString(";\n") + ";\n" +
      privmeths.map(_.signature(this)).mkString(";\n") + ";\n" +
      "};\n"
  }

  def staticStruct = {
    val meths = pubmeths ++ privmeths
    if (meths.length > 0) {
      "struct _" + ref + "_meth " + name + " = {" + header + "_alloc, " + header + "_dealloc," + meths.map(_.nm(header)).mkString(", ") + "};\n"
    } else {
      "struct _" + ref + "_meth " + name + " = {" + header + "_alloc, " + header + "_dealloc};\n"
    }
  }

  def methods() : String = {
    pubmeths.map(_.toString(this)).mkString("\n") + "\n" +
    privmeths.map(_.toString(this)).mkString("\n") + "\n"
  }

  def prototypes() : String = {

    ref + " " + header + "_alloc();\n" +
    "void " + header + "_dealloc(" + ref + " self);\n" +
    pubmeths.map(_.prototype(this)).mkString("\n") + "\n" +
    privmeths.map(_.prototype(this)).mkString("\n") + "\n"
  }
}

class Comp extends RegexParsers with PackratParsers {

  lazy val ident: PackratParser[String] = "[A-Za-z][A-Za-z0-9_]*".r ^^ {
    case s => s
  }

  lazy val typename: PackratParser[String] = "[A-Za-z][A-Za-z0-9_]*".r ^^ {
    case s => s
  }

  lazy val typed: PackratParser[Typed] = typename ~ ident ^^ {
    case a ~ b => Typed(a, b)
  }

  lazy val param: PackratParser[Typed] = typed <~ ";" ^^ {
    case a => a
  }

  lazy val param_list: PackratParser[List[Typed]] = repsep(typed, ",")

  lazy val method: PackratParser[Method] = typed ~ ('(' ~> param_list <~ ')') ~ matching ^^ {
    case t ~ ls ~ src => Method(t, ls, src)
  }

  lazy val bracefree: PackratParser[String] = """[^{}]+""".r ^^ {
    case a => a
  }

  lazy val matching: PackratParser[String] = "{" ~> rep(bracefree | matching) <~ "}" ^^ {
    case b => "{\n" + b.mkString("") + "}"
  }

  lazy val features: PackratParser[List[Param]] = rep(param | method)

  lazy val CClass: PackratParser[Class] =
      ("class" ~> ident <~ ":") ~ ("{:public" ~> features <~ "}") ~
      ("{:private" ~> features <~ "}") ^^ {
    case nm ~ pub ~ priv => Class(nm, pub, priv)
  }

  lazy val include: PackratParser[Include] = "#[a-z]+ [^\n]+".r ^^ {
    case s => Include(s)
  }
  
  lazy val file: PackratParser[List[TopLevel]] = rep(include | CClass)

}

object Preproc extends Comp {
  def main(args: Array[String])= {
    val sauce = io.Source.stdin.mkString
    println(parseAll(file, sauce).get.mkString("\n"))
  }
}
