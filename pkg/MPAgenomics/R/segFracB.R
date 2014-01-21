#'
#' This function launches the segmentation PELT (from package changepoint) with a penalty value of lambda * log(n) for a range of value for lambda.
#' Then an optimal penalty value will be choose in looking for a stabilization in the number of segments according to the penalty values.
#'
#' @title segmentation function for the allele B fraction
#'
#' @param dataSetName name of the dataset (it must correpond to a folder name in rawData folder.).
#' @param normalTumorArray only if you have normal and tumor profile in your data folder. A csv file or a data.frame with 2 columns: "normal" and "tumor".
#' The first column contains the name of normal files and the second the names of associated tumor files.
#' @param chromosome A vector containing the chromosome which the CN signal must be extract. 
#' @param Lambda sequence of penalty parameters to test
#' @param listOfFiles vector containing the names of the files in dataSetName to extract.
#' @param savePlot if TRUE, plot the segmentation results
#' @param verbose if TRUE print some informations
#' 
#' @return a data.frame where each row correspond to a different segment with columns :
#' \describe{
#'   \item{sampleNames}{The name of the signal.}
#'   \item{chromosome}{A vector of the same size as copynumber containing the chromosome number.}
#'   \item{chromStart}{The starting position of a segment.}
#'   \item{chromEnd}{The ending position of a segment.}
#'   \item{probes}{The number of probes in the segment.}
#'   \item{means}{Means of the segment.}
#' }
#'
#' @export
segFracBSignal=function(dataSetName,normalTumorArray,chromosome=1:22,Lambda=NULL,listOfFiles=NULL,savePlot=TRUE,verbose=TRUE)
{
  allpkg=TRUE
  if(!suppressPackageStartupMessages(require("aroma.cn", quietly=TRUE) ) )
  {
    cat("Package not found: aroma.affymetrix. For download it:\n")
    cat("source(\"http://www.braju.com/R/hbLite.R\")\n")
    cat(" hbLite(\"sfit\")\n")
    cat("source(\"http://bioconductor.org/biocLite.R\")\n")
    cat("biocLite(\"affxparser\")\n")
    cat("source(\"http://aroma-project.org/hbLite.R\")\n")
    cat("hbInstall(\"aroma.affymetrix\")\n")
    cat("hbInstall(\"aroma.cn\")\n") 
    allpkg=FALSE
  }
#  else
#    cat("Package aroma.cn loaded.\n")
  
  if(!suppressPackageStartupMessages(require("changepoint",quietly=TRUE)))
  {
    allpkg=FALSE
    cat("The package changepoint is missing. You can install it with the following command:\n","install.packages(\"changepoint\") \n")
  }
#  else
 #   cat("Package changepoint loaded.\n")
  
  if(!allpkg)
    stop("You have to install some packages : Follow the printed informations.")
  
  
  if(!("totalAndFracBData"%in%list.files()))
    stop("There is no \"totalAndFracBData\", check if you are in the good working directory or if you have run the signalPreProcess function before.")
  if(!("callData"%in%list.files()))
    stop("There is no \"totalAndFracBData\", check if you are in the good working directory or if you have run the signalPreProcess function before.")
  
  ###check the arguments
  #dataSetName
  if(missing(dataSetName))
    stop("dataSetName is missing.")
  if(!is.character(dataSetName))
    stop("dataSetName must be the name of a folder in \"rawData\" folder.")
  if(!(dataSetName%in%list.files("rawData")))
    stop("dataSetName must be the name of a folder in \"rawData\" folder.")

  #################### import the dataSet to have the name of all the files
  #check if we are in a normal-tumor study or in a single array study
  singleStudy=TRUE
  if(!missing(normalTumorArray))
    singleStudy=FALSE
  
  #path where find the CN data
  rootPath <- "totalAndFracBData";
  rootPath <- Arguments$getReadablePath(rootPath);
  dataSet <- paste0(dataSetName,",ACC,ra,-XY,BPN,-XY,AVG,FLN,-XY");
  
  #load CN
  dsC <- AromaUnitTotalCnBinarySet$byName(dataSet, chipType="*", paths=rootPath);
    
  ################### check normalTumorArray
  if(!singleStudy)
  {
    #normalTumorArray
    if(is.character(normalTumorArray))
      normalTumorArray=read.csv(normalTumorArray)
    else
    {
      if(!is.data.frame(normalTumorArray))
        stop("normalTumorArray must be either the path to the normalTumorArray csv file or a data.frame containing the data.\n")
    }
    
    #check normalTumorArray
    if(!("normal"%in%colnames(normalTumorArray)) || !("tumor"%in%colnames(normalTumorArray)))
      stop("normalTumorArray doesn't contain a column \"normal\" or \"tumor\".\n")
    
#     isArrayComplete=sapply(getNames(dsC),FUN=function(name,listOfNames){name%in%listOfNames},c(as.character(normalTumorArray$normal),as.character(normalTumorArray$tumor)))
#     if(sum(isArrayComplete)!=length(isArrayComplete))
#       warning("normalTumorArray doesn't contain all the filenames of dataSetName.")
  }
  
  ###### check listOfFiles
  pos=c()
  if(is.null(listOfFiles) || missing(listOfFiles))
  {
    listOfFiles=getNames(dsC)
    pos=1:length(dsC)
  }
  else
  {
    #check the format
    if(!is.vector(listOfFiles) || !is.character(listOfFiles))
      stop("listOfFiles must be a vector of string.")
    listOfFiles=unique(listOfFiles)
    #check if all the files of listOfFiles are in the folder
    pos=sapply(listOfFiles,match,getNames(dsC))#position of the files of listOfFiles in the folder
    if(length(which(pos>0))!=length(pos))
      stop("Wrong name of files in listOfFiles")
    
    if(!missing(normalTumorArray))
    {
      isArrayComplete=sapply(listOfFiles,FUN=function(name,listOfNames){name%in%listOfNames},c(as.character(normalTumorArray$normal),as.character(normalTumorArray$tumor)))
      if(sum(isArrayComplete)!=length(isArrayComplete))
        stop("normalTumorArray doesn't contain all the filenames you specified in listOfFiles parameter.")
    }

  }  
  
  #if single array study, we just reduce dsC to pos  
#   if(!singleStudy)
#   {  
#     #if normal-tumor study, we need the tumor and normal files
#     
#     #we obtain the complementary files  
#     compFiles=getComplementaryFile(listOfFiles,normalTumorArray)
#     allFiles=unique(c(listOfFiles,compFiles))
#     
#     #index of the files
#     pos=sapply(allFiles,FUN=function(x,dsC){which(getNames(dsC)==x)},dsC)
#     tag=getStatus(allFiles,normalTumorArray)
#     
#     pos=pos[which(tag=="normal")]
#   }
  
  #Lambda
  if(missing(Lambda) || is.null(Lambda))
    Lambda=c(seq(0.1,2,by=0.1),seq(2.2,5,by=0.2),seq(5.5,10,by=0.2),seq(11,16,by=1),seq(18,36,by=2),seq(40,80,by=4))
  
  ######################### END CHECK PARAMETERS
  ###bed files output
  #results are stored in the segmentation folder
  #check the existence of the segmentation folder
  if(!("segmentation"%in%list.files()))
    dir.create("segmentation");
  #check the existence of th dataSet folder in segmentation folder
  if(!(dataSetName%in%list.files("segmentation")))
    dir.create(paste0("segmentation/",dataSetName));
  
  figPath <- Arguments$getWritablePath(paste0("figures/",dataSetName,"/segmentation/fracB"));
  
  #names of the files to segment
  names=getNames(dsC)[pos]
  
  output=lapply(names,FUN=function(name)
  {
    segment=data.frame()
    for(chr in chromosome)
    {
      #get the fracB for 1 chr
      if(!singleStudy)
        fracB=getFracBSignal(dataSetName,chromosome=chr,normalTumorArray,listOfFiles=name,verbose=FALSE)
      else
        fracB=getFracBSignal(dataSetName,chromosome=chr,listOfFiles=name,verbose=FALSE)

      fracB=fracB[[paste0("chr",chr)]]$tumor
      gc()
      #get the genotype calls for 1 chr
      geno=getGenotypeCalls(dataSetName,chromosome=chr,listOfFiles=name,verbose=FALSE)
      geno=geno[[paste0("chr",chr)]]
      gc()
      
      ind=which(geno[,3]=="AB")

      rm(geno)
      gc()
      
      fracB=fracB[ind,]

      fracB[,3]=symmetrizeFracB(fracB[,3])

      #segmentation
      cat(paste0("Segmentation of file ",name," chromosome ",chr,"..."))
      seg=PELT(fracB[,3],Lambda,position=fracB$position,plot=TRUE,verbose=FALSE)
      cat("OK\n")
      
      if(savePlot)
      {
        figName <- sprintf("%s,%s,%s", name, "fracB,chr",chr);
        pathname <- filePath(figPath, sprintf("%s.png", figName));
        width <- 1280;
        aspect <- 0.6*1/3;
        fig <- devNew("png", pathname, label=figName, width=width, height=2*aspect*width);
        plot(NA,xlim=c(min(fracB$position),max(fracB$position)), ylim=c(0,1),xlab="Position", main=figName,ylab="Allele B fraction", pch=".")
        points(fracB$position, fracB[,3], pch=".");
        for(i in 1:nrow(seg$segment))
          lines(c(seg$segment$start[i],seg$segment$end[i]),rep(seg$segment$means[i],2),col="red",lwd=3)
        devDone();
      }
      
      
      
      #concatenate the results
#       cghArg=list(fracB=rbind(cghArg$copynumber,seg$signal),
#                   segmented=rbind(cghArg$segmented,seg$segmented),
#                   startPos=c(cghArg$startPos,fracB$position),
#                   chromosome=c(cghArg$chromosome,rep(fracB$chromosome,length(seg$signal))),
#                   sampleNames=name,
#                   featureNames=c(cghArg$featureNames,fracB$featureNames),
#                   segment=data.frame(chrom=c(as.character(cghArg$segment$chrom),unlist(lapply(rep(fracB$chromosome,length(seg$segment$means)),as.character))),
#                                      chromStart=c(cghArg$segment$chromStart,seg$segment$start),
#                                      chromEnd=c(cghArg$segment$chromEnd,seg$segment$end),
#                                      probes=c(cghArg$segment$probes,seg$segment$points),
#                                      means=c(cghArg$segment$means,seg$segment$means)))
      
        segment=data.frame(chrom=c(as.character(segment$chrom),rep(paste0("chr",chr),length(seg$segment$start))),
                           chromStart=c(segment$chromStart,seg$segment$start),
                           chromEnd=c(segment$chromEnd,seg$segment$end),
                           probes=c(segment$probes,seg$segment$points),
                           means=c(segment$means,seg$segment$means))
    }
    
    
    #write in .bed
    write.table(segment,file=paste0("segmentation/",dataSetName,"/",name,",symFracB,segmentation.bed"),sep="\t",row.names=FALSE)
    segment=cbind(rep(name,nrow(segment)),segment)
    names(segment)[1]="sampleNames"
    return(segment)
  })
  
  output=do.call(rbind,output)
  row.names(output)=NULL
  
  return(output)            
}