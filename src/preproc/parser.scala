import util.parsing.combinator._

abstract class Param {
  def toString(cls: String) : String
  def nm(cls: String) : String = cls + "_" + nm
  val nm : String
}
case class Typed(ty: String, nm: String) extends Param {
  override def toString() = ty + " " + nm
  def toString(cls: String) : String = ty + " " + nm(cls)
}

case class Method(ty: Typed, args: List[Typed], src: String) extends Param { 

  val nm = ty.nm
  override def toString() = ty + "(" + args.mkString(",") + ")" + src
  def toString(cls: String) : String = ty.toString(cls) + "(" + args.mkString(",") + ")" + src
  def signature : String = ty.ty + "(*" + nm + ") (" + args.mkString(",") + ")"
}

abstract class TopLevel

case class Include(include: String) extends TopLevel {
  override def toString() = include
}

case class Class(name: String, pubs: List[Param], priv: List[Param]) extends TopLevel {

  val header = "_" + name + "_h"
  val pubmeths = pubs.filter(_.isInstanceOf[Method]).map(_.asInstanceOf[Method])
  val ref = name + "Ref"

  override def toString() = {
    "#ifndef " + header + "\n" +
    "#define " + header + "\n" +
    classStruct +
    allocs() +
    methods() +
    staticStruct +
    "#endif //" + header + "\n"
  }

  def allocs() = {
    ref + "\n" +
    header + "_alloc() {\n" +
      "return (" + ref + ") malloc(sizeof(" + ref + "));\n" +
    "}\n\n" +
    "void\n" +
    header + "_dealloc(" + ref + " self) {\n" +
      "free(self);\n" +
    "}\n"
  }

  def classStruct : String = {
    //TODO alloc/dealloc
    "typedef struct _" + ref + " {\n" +
    pubs.filter(_.isInstanceOf[Typed]).mkString(";\n") + ";\n" +
    "} *" + ref + ";\n" +
    "struct _" + ref + "_meth {\n" +
    //TODO Proper struct thing
    ref +" (*alloc) ();\n" +
    "void (*dealloc) ();\n" +
    pubmeths.map(_.signature).mkString(";\n") + ";\n" +
    "};\n"
  }

  def staticStruct = {
    //TODO alloc/dealloc
    if (pubmeths.length > 0) {
      "struct _" + ref + "_meth " + name + " = {" + header + "_alloc, " + header + "_dealloc," + pubmeths.map(_.nm(header)).mkString(", ") + "};\n"
    } else {
      "struct _" + ref + "_meth " + name + " = {alloc, dealloc};\n"
    }
  }

  def methods() : String = {
    //TODO alloc
    pubmeths.map(_.toString(header)).mkString("\n") + "\n" +
    priv.filter(_.isInstanceOf[Method]).map(_.toString(header)).mkString("\n") + "\n"
  }
}

class Comp extends RegexParsers with PackratParsers {

  lazy val ident: PackratParser[String] = "[A-Za-z0-9_]+".r ^^ {
    case s => s
  }

  lazy val typename: PackratParser[String] = "[A-Za-z0-9_]+".r ^^ {
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
