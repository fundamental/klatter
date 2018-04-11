function myplot(xdata, ydata, sym, ttl, xlab, ylab, grd, ...
                 lgnd, linewidth, fontsize)
% MYPLOT - Generic plot - compatibility wrapper for plot()

  if nargin<10, fontsize=12; end
  if nargin<9, linewidth=1; end
  if nargin<8, lgnd=''; end
  if nargin<7, grd=1; end
  if nargin<6, ylab=''; end
  if nargin<5, xlab=''; end
  if nargin<4, ttl=''; end
  if nargin<3, sym=''; end
  if nargin<2, ydata=xdata; xdata=0:length(ydata)-1; end

  plot(xdata,ydata,sym,'linewidth',linewidth); 
  if length(ttl)>0, title(ttl,'fontsize',fontsize,...
                              'fontname','helvetica'); 
  end
  if length(ylab)>0, ylabel(ylab,'fontsize',fontsize,...
                                 'fontname','helvetica'); 
  end
  if length(xlab)>0, xlabel(xlab,'fontsize',fontsize,...
                                 'fontname','helvetica'); 
  end
  if grd, grid('on'); else grid('off'); end
  if length(lgnd)>0, legend(lgnd); end
