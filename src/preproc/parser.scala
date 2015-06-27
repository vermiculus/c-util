/**
 *@author: Josh Snider
 */

import util.parsing.combinator._

abstract class Param {
  def toString(cls: Class) : String
  def nm(cls: String) : String = cls + "_" + nm
  val nm : String
}
case class Typed(ty: List[String], nm: String) extends Param {
  val isPointer = ty.find(_.contains("*")) != None
  override def toString() = ty.mkString(" ") + " " + nm
  def toString(cls: Class) : String = ty.mkString(" ") + " " + nm(cls.header)
}

case class Method(ty: Typed, args: List[Typed], src: String) extends Param { 

  val nm = ty.nm
  override def toString() = ty + "(" + args.mkString(",") + ")" + src
  def argsString(cls: Class) : String = "(" + (List(Typed(List(cls.ref), "self")) ++ args).mkString(",") + ")"
  def toString(cls: Class) : String = ty.toString(cls) + argsString(cls) + src
  def prototype(cls: Class) : String = ty.toString(cls) + argsString(cls) + ";"
  def signature(cls: Class) : String = {
    ty.ty.mkString(" ") + "(*" + nm + ") " + argsString(cls)
  }
}

abstract class TopLevel

case class Include(include: String) extends TopLevel {
  override def toString() = include
}

case class Class(name: String, pubs: List[Param], priv: List[Param]) extends TopLevel {

  val attrs = (pubs ++ priv).filter(_.isInstanceOf[Typed]).map(_.asInstanceOf[Typed])
  val header = "_" + name + "_h"
  val privmeths = priv.filter(_.isInstanceOf[Method]).map(_.asInstanceOf[Method])
  val pubmeths = pubs.filter(_.isInstanceOf[Method]).map(_.asInstanceOf[Method])
  val ref = name + "Ref"

  override def toString() = {
    "#ifndef " + header + "\n" +
      "#define " + header + "\n" +
      "//Class file for " + name + "\n" +
      "//Produced by c-util.\n" +
      "//Search GitHub for more information.\n" +
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
      attrs.filter(_.isPointer).map(a => "free(self->" + a.nm + ");\n").mkString("") +
      "free(self);\n" +
      "}\n"
  }

  def classStruct : String = {
    "typedef struct _" + ref + " {\n" +
      pubs.filter(_.isInstanceOf[Typed]).mkString(";\n") + ";\n" +
      "} *" + ref + ";\n" +
      "struct _" + ref + "_meth {\n" +
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

  lazy val typename: PackratParser[String] =
      "([A-Za-z][A-Za-z0-9_]*)|(\\[\\s*\\])|\\*".r ^^ {
    case s => s
  }

  lazy val typed: PackratParser[Typed] = rep1(typename)  ^^ {
    case a => Typed(a.reverse.tail.reverse, a.last)
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
